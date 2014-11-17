var system = require('system');
var fs = require('fs');

if (system.args.length != 3) {
    console.log("Usage: generate-bbox.js svg_input_file bbox_output_file ");
    phantom.exit(1);
}

var address = system.args[1];
var output = system.args[2];
var page = require('webpage').create();

function serialize() {
    var items = document.documentElement.getElementsByTagName('path');
    var bb = null;
    var getScreenBBox_impl = null;
    var svgns = "http://www.w3.org/2000/svg";
    
    function getScreenBBox(e) {
        if (e.getScreenBBox) {
            return e.getScreenBBox();
        } else if (getScreenBBox_impl) {
            return getScreenBBox_impl(e);
        }
    }
    
    var header = "/*\n\
    * This list is used for getting the bounding box of the Leipzig glyphs.\n\
    * The values were obtained with ./varia/svg/split.xsl and boundingbox.svg.\n\
    * It should not be modified by hand.\n\
    */\n";
    var impl = header;

    var i;
    for (i = 0; i < items.length; i++) {
        item = items[i];
        label = "LEIPZIG_BBOX_" + item.getAttribute("id").toUpperCase();
        r = item.getBBox();
        
        // add the define to the header
        header += "#define " + label + " " + i + "\n";
        
        // add the bb value to the implementation
        impl += "m_bBox[" + label + "].m_x = " + r.x.toFixed(1) + ";\n";
        impl += "m_bBox[" + label + "].m_y = " + r.y.toFixed(1) + ";\n";
        impl += "m_bBox[" + label + "].m_width = " + r.width.toFixed(1) + ";\n";
        impl += "m_bBox[" + label + "].m_height = " + r.height.toFixed(1) + ";\n";
    }
    
    header += "#define LEIPZIG_GLYPHS " + i + "\n";
    header += "/* end of the generated data */\n\n";
    impl += "/* end of the generated data */\n\n";
    
    return [impl, header];
}

function extract() {
    return function (status) {
        if (status != 'success') {
            console.log("Failed to open the page.");
        } else {
            var code = page.evaluate(serialize);
            try {
                // We write the impl to the file...
                fs.write(output, code[0], 'w');
            } catch(e) {
                console.log(e);
            }
            // ... and log the header
            console.log(code[1]);
        }
        phantom.exit();
    };
}

page.open(address, extract());