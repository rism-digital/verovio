/************************************************************************
 * Circular Web Audio Buffer Queue
 */
function CircularAudioBuffer(slots) {
    slots = slots || 24;
    // number of buffers
    this.slots = slots;
    this.buffers = new Array(slots);
    
    this.reset();
    
    for (var i = 0; i < this.slots; i++) {
        var buffer = audioCtx.createBuffer(channels, BUFFER, SAMPLE_RATE);
        this.buffers[i] = buffer;
    }
}

// pseudo empty all buffers
CircularAudioBuffer.prototype.reset = function () {
    this.used = 0;
    this.filled = 0;
};

// returns number of buffers that are filled
CircularAudioBuffer.prototype.filledBuffers = function () {
    var fills = this.filled - this.used;
    if (fills < 0) fills += this.slots;
    return fills;
}

// returns whether buffers are all filled
CircularAudioBuffer.prototype.full = function () {
    //console.debug(this.filledBuffers());
    return this.filledBuffers() >= this.slots - 1;
}

// returns a reference to next available buffer to be filled
CircularAudioBuffer.prototype.prepare = function () {
    if (this.full()) {
        //console.log('buffers full!!')
        return
    }
    var buffer = this.buffers[ this.filled++];
    this.filled %= this.slots;
    return buffer;
}

// returns the next buffer in the queue
CircularAudioBuffer.prototype.use = function () {
    if (! this.filledBuffers()) return;
    var buffer = this.buffers[ this.used++];
    this.used %= this.slots;
    return buffer;
}

/************************************************************************
 * Web Audio Stuff
 */

var SAMPLE_RATE = 44100;
var BUFFER = 4096;
var channels = 2;

var audioCtx;
var source;
var scriptNode;
var circularBuffer;
var emptyBuffer;

function initAudio() {
    audioCtx = new (window.AudioContext || window.webkitAudioContext)();
    scriptNode = audioCtx.createScriptProcessor(BUFFER, 0, channels);
    scriptNode.onaudioprocess = onAudioProcess;
    
    source = audioCtx.createBufferSource();
    circularBuffer = new CircularAudioBuffer(8);
    emptyBuffer = audioCtx.createBuffer(channels, BUFFER, SAMPLE_RATE);
    
    source.connect(scriptNode);
    source.start(0);
    console.debug("initAudio");
}

function startAudio() {    
    scriptNode.connect(audioCtx.destination);
    console.debug("startAudio");
}

function pauseAudio() {
    circularBuffer.reset();
    scriptNode.disconnect();
	console.debug("pauseAudio");
}


/************************************************************************
 * Emscripten variables and callback - cannot be renamed
 */

var ULONG_MAX = 4294967295;
var _EM_signalStop = 0;
var _EM_seekSamples = ULONG_MAX;

function processAudio(buffer_loc, size) {
    var buffer = circularBuffer.prepare();
    var left_buffer_f32 = buffer.getChannelData(0);
    var right_buffer_f32 = buffer.getChannelData(1);
        
    // Copy emscripten memory (OpenAL stereo16 format) to JS
    for (var i = 0; i < size; i++) {
        left_buffer_f32[i] = MidiPlayer.HEAP16[(buffer_loc >> 1) + 2 * i + 0] / 32768;
        right_buffer_f32[i] = MidiPlayer.HEAP16[(buffer_loc >> 1) + 2 * i + 1] / 32768;
    }
}

function updateProgress(current, total) {
    midiPlayer_currentSamples = current;
    midiPlayer_totalSamples = total;
    midiPlayer_progress.style.width = (current / total * 100) + '%';
    midiPlayer_playingTime.innerHTML = samplesToTime(current);
    midiPlayer_totalTime.innerHTML = samplesToTime(total);
    
    var millisec = Math.floor(current * 1000 / SAMPLE_RATE / midiPlayer_updateRate);
    if (midiPlayer_lastMillisec > millisec) {
        midiPlayer_lastMillisec = 0;
    }
    if (millisec > midiPlayer_lastMillisec) {
        if (midiPlayer_onUpdate != null) midiPlayer_onUpdate(millisec * midiPlayer_updateRate);
        //console.log(millisec * UPDATE_RATE);
    }
    midiPlayer_lastMillisec = millisec;
}

function completeConversion(status) {
    midiPlayer_drainBuffer = true;
    console.debug('completeConversion');
    midiPlayer_convertionJob = null;
    // Not a pause
    if (_EM_signalStop != 2) {
        setTimeout(stop, 1000);   
    }
}

/************************************************************************
 * Global player variables and functions
 */

// html elements
var midiPlayer_width;
var midiPlayer_bar;
var midiPlayer_progress;
var midiPlayer_playingTime;
var midiPlayer_play;
var midiPlayer_pause;
var midiPlayer_stop;
var midiPlayer_totalTime;

// variables
var midiPlayer_isLoaded = false;
var midiPlayer_isAudioInit = false;
var midiPlayer_input = null;
var midiPlayer_lastMillisec = 0;
var midiPlayer_midiName = ''
var midiPlayer_convertionJob = null;
var midiPlayer_currentSamples = ULONG_MAX;
var midiPlayer_totalSamples = 0;
var midiPlayer_updateRate = 50;
var midiPlayer_drainBuffer = false;
var BASE64_MARKER = ';base64,';

// callbacks
var midiPlayer_onStop = null;
var midiPlayer_onUpdate = null;

var MidiPlayer = {
    noInitialRun: true,
    totalDependencies: 1,
    monitorRunDependencies: function(left) {
        //console.log(this.totalDependencies);
        //console.log(left);
        if ((left == 0) && !midiPlayer_isLoaded) {
          console.log("MidiPlayer is loaded");
          midiPlayer_isLoaded = true;
        }
    }
};
MidiModule(MidiPlayer); 

 
function onAudioProcess(audioProcessingEvent) {
    var generated = circularBuffer.use();
    
    if (!generated && midiPlayer_drainBuffer) {
        // wait for remaining buffer to drain before disconnect audio
        pauseAudio();
        midiPlayer_drainBuffer = false;
        return;
    }
    if (!generated) {
        //console.log('buffer under run!!')
        generated = emptyBuffer;
    }
    
    var outputBuffer = audioProcessingEvent.outputBuffer;
    var offset = 0;
    if (outputBuffer.copyToChannel !== undefined) {
        // Firefox -> about 50% faster than decoding
        outputBuffer.copyToChannel(generated.getChannelData(0), 0, offset);
        outputBuffer.copyToChannel(generated.getChannelData(1), 1, offset);
    } else {
        // Other browsers -> about 20 - 70% slower than decoding
        var leftChannel = outputBuffer.getChannelData(0);
        var rightChannel = outputBuffer.getChannelData(1);
        var generatedLeftChannel = generated.getChannelData(0);
        var generatedRightChannel = generated.getChannelData(1);
        var i;
        for (i = 0; i < BUFFER; i++) {
            leftChannel[i] = generatedLeftChannel[i];
            rightChannel[i] = generatedRightChannel[i];
        }
    }
}

function samplesToTime(at) {
    var in_s = Math.floor(at / SAMPLE_RATE);
    var s = in_s % 60;
    var min = in_s / 60 | 0;
    return min + ':' + (s === 0 ? '00': s < 10 ? '0' + s: s);
}

function convertDataURIToBinary(dataURI) {
    var base64Index = dataURI.indexOf(BASE64_MARKER) + BASE64_MARKER.length;
    var base64 = dataURI.substring(base64Index);
    var raw = window.atob(base64);
    var rawLength = raw.length;
    var array = new Uint8Array(new ArrayBuffer(rawLength));
    
    for (var i = 0; i < rawLength; i++) {
        array[i] = raw.charCodeAt(i);
    }
    return array;
}

function convertFile(file, data) {
    midiPlayer_midiName = file;
    midiPlayer_input = null;
    console.log('open ', midiPlayer_midiName);
    MidiPlayer['FS'].writeFile(midiPlayer_midiName, data, {
        encoding: 'binary'
    });
    play();
}

function pause() {
    _EM_signalStop = 2;
    circularBuffer.reset();
    midiPlayer_play.style.display = 'inline-block';
    midiPlayer_pause.style.display = 'none';
}

function play() {
    if (!midiPlayer_isLoaded) {
        console.error("MidiPlayer is not loaded yet");
        return;
    }
    if (!midiPlayer_isAudioInit) {
          initAudio();
          midiPlayer_isAudioInit = true;
    }

    _EM_seekSamples = midiPlayer_currentSamples;
    if (midiPlayer_convertionJob) {
        return;
    }
    
    _EM_signalStop = 0;
    midiPlayer_play.style.display = 'none';
    midiPlayer_pause.style.display = 'inline-block';
    midiPlayer_stop.style.display = 'inline-block';
    // add small delay so UI can update.
    setTimeout(runConversion, 100);
}

function stop() {
    _EM_signalStop = 1;
    _EM_seekSamples = 0;
    circularBuffer.reset();
    
    midiPlayer_totalSamples = 0;
    midiPlayer_currentSamples = ULONG_MAX;
    midiPlayer_progress.style.width = '0%';
    midiPlayer_playingTime.innerHTML = "00.00";
    midiPlayer_totalTime.innerHTML = "00.00";
    
    midiPlayer_play.style.display = 'none';
    midiPlayer_pause.style.display = 'none';
    midiPlayer_stop.style.display = 'none';
    
    if (midiPlayer_onStop != null) midiPlayer_onStop(); 
}

function runConversion() {
    midiPlayer_convertionJob = {
        sourceMidi: midiPlayer_midiName,
        targetWav: midiPlayer_midiName.replace(/\.midi?$/i, '.wav'),
        targetPath: '',
        conversion_start: Date.now()
    };
    
    var sleep = 10;
    circularBuffer.reset();
    startAudio();

    console.log(midiPlayer_convertionJob);
        
    MidiPlayer.ccall('wildwebmidi',
        null,[ 'string', 'string', 'number'],[midiPlayer_convertionJob.sourceMidi, midiPlayer_convertionJob.targetPath, sleep], {
            async: true
        });
}

/************************************************************************
 * jQuery player plugin 
 */

(function ($) {
    
    $.fn.midiPlayer = function (options) {

        var options = $.extend({
            // These are the defaults.
            color: "#556b2f",
            backgroundColor: "white",
            width: 500,
            onStop: null,
            onUpdate: null,
            updateRate: 50,
        },
        options);
        // width should not be less than 150
        options.width = Math.max(options.width, 150);
        // update rate should not be less than 10 milliseconds
        options.updateRate = Math.max(options.updateRate, 10);
        
        $.fn.midiPlayer.play = function (song) {
            if (midiPlayer_isLoaded == false) {
                midiPlayer_input = song;
            }
            else {
                var byteArray = convertDataURIToBinary(song);
                if (midiPlayer_totalSamples > 0) {
                    stop();
                    // a timeout is necessary because otherwise writing to the disk is not done
                    setTimeout(function() {convertFile("player.midi", byteArray);}, 200);
                }
                else {
                    convertFile("player.midi", byteArray);
                }
            }
        };
        
        $.fn.midiPlayer.seek = function (millisec) {
            if (midiPlayer_totalSamples == 0) return;
            var samples = millisec * SAMPLE_RATE / 1000;
            midiPlayer_currentSamples = Math.min(midiPlayer_totalSamples, samples);
            play();
        };
        
        $.fn.midiPlayer.stop = function () {
            stop();
        };
        
        // Create the player
        this.append("<div id=\"midiPlayer_div\"></div>");
        $("#midiPlayer_div").append("<div id=\"midiPlayer_playingTime\">0:00</div>")
            .append("<div id=\"midiPlayer_bar\"><div id=\"midiPlayer_progress\"></div></div>")
            .append("<div id=\"midiPlayer_totalTime\">0:00</div>")
            .append("<a class=\"icon play\" id=\"midiPlayer_play\" onclick=\"play()\"></a>")
            .append("<a class=\"icon pause\" id=\"midiPlayer_pause\" onclick=\"pause()\"></a>")
            .append("<a class=\"icon stop\" id=\"midiPlayer_stop\" onclick=\"stop()\"></a>");
            
        $("#midiPlayer_div").css("width", options.width + 200);
        $("#midiPlayer_bar").css("width", options.width);
        $("#midiPlayer_progress").css("background", options.color);
        
        // Assign the global variables
        midiPlayer_onStop = options.onStop;
        midiPlayer_onUpdate = options.onUpdate; 
        midiPlayer_updateRate = options.updateRate;
        midiPlayer_bar = document.getElementById('midiPlayer_bar');
        midiPlayer_progress = document.getElementById('midiPlayer_progress');
        midiPlayer_playingTime = document.getElementById('midiPlayer_playingTime');
        midiPlayer_play = document.getElementById('midiPlayer_play');
        midiPlayer_pause = document.getElementById('midiPlayer_pause');
        midiPlayer_stop = document.getElementById('midiPlayer_stop');
        midiPlayer_totalTime = document.getElementById('midiPlayer_totalTime');
        
        
        var pageDragStart = 0;
        var barDragStart = 0;
        midiPlayer_bar.addEventListener('mousedown', function (e) {
            if (midiPlayer_totalSamples == 0) return;
            pageDragStart = e.pageX;
            barDragStart = e.offsetX;
            updateDragging(e.pageX);
        });
        window.addEventListener('mousemove', function (e) {
            if (pageDragStart != 0) {
                pause();
                updateDragging(e.pageX);
            }
            
        });
        window.addEventListener('mouseup', function (e) {
            if (pageDragStart == 0) return;
            if (midiPlayer_totalSamples == 0) return;
            pageDragStart = 0;
            play();
        });
        
        function updateDragging(pageX) {
            var posX =  barDragStart + (pageX - pageDragStart);
            if (posX >= 0 && posX <= options.width) {
                var percent = posX / options.width;
                midiPlayer_currentSamples = percent * midiPlayer_totalSamples | 0;
                updateProgress(midiPlayer_currentSamples, midiPlayer_totalSamples);
            }
        }
        
        return;
    };
}
(jQuery));