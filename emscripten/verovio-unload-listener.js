// add a listener that will delete the object (if necessary) when the page is left
window.addEventListener ("unload", function () {
	if (verovio.ptr != 0) {
		verovio.vrvToolkit.destructor( verovio.ptr );
	}
});