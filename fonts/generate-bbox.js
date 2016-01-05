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
    var unitsPerEm = document.documentElement.getElementsByTagName('font-face')[0].getAttribute("units-per-em");
    
    function getScreenBBox(e) {
        if (e.getScreenBBox) {
            return e.getScreenBBox();
        } else if (getScreenBBox_impl) {
            return getScreenBBox_impl(e);
        }
    }
    
    var impl = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    impl += "<bounding-boxes ";
    impl += "font-family=\"" + document.documentElement.getAttribute("font-family") + "\" ";
    impl += "units-per-em=\"" + unitsPerEm + "\">\n";

    var i;
    for (i = 0; i < items.length; i++) {
        item = items[i];
        impl += "   <g c=\"" + item.getAttribute("id").toUpperCase() + "\" "; 
        r = item.getBBox();
        
        // add the bb value to the implementation
        impl += "x=\"" + r.x.toFixed(1) + "\" ";
        impl += "y=\"" + r.y.toFixed(1) + "\" ";
        impl += "w=\"" + r.width.toFixed(1) + "\" ";
        impl += "h=\"" + r.height.toFixed(1) + "\" ";
        
        impl += "/>\n";
    }
    
    impl += "</bounding-boxes>\n";
    
    return impl;
}

function extract() {
    return function (status) {
        if (status != 'success') {
            console.log("Failed to open the page.");
        } else {
            var code = page.evaluate(serialize);
            try {
                // We write the impl to the file...
                fs.write(output, code, 'w');
            } catch(e) {
                console.log(e);
            }
            // ... and log the header
            //console.log(code[1]);
        }
        phantom.exit();
    };
}

page.open(address, extract());