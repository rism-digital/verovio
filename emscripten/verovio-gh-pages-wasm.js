// code to be added to the verovio-toolkit-wasm.js for the gh-pages mei-viewer-wasm.xhtml experimental page to work
console.log("Running added code in verovio-toolkit-wasm.js");

var vrvToolkit = new verovio.toolkit();

// Load the default file or the file passed in the URL
var file = getParameterByName("file");

if (!file || (file.length === 0)) {
    file = "examples/downloads/Chopin_Etude_op.10_no.9.mei";
}

$.ajax({
    url: file
    , dataType: "text"
    , success(data) {
        var outputFilename = file.replace(/^.*[\\\/]/, "");
        load_data( data );
    }
});
