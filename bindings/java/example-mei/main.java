import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.io.IOException;

/* import the toolkit */
import org.rismch.verovio.toolkit;
import org.rismch.verovio.NativeUtils;

public class main {
	public static void main(String argv[]) {
		try {
			NativeUtils.loadLibraryFromJar("/META-INF/lib/libverovio.jnilib");
		} catch (IOException e) {
			System.out.println("Could not load Verovio native lib: " + e.getMessage());
		}
        
        /* 
         * create the toolkit and set the resource path
         * (if the fonts are installed (see the ./tools) then there is not need to do this)
         */
		toolkit vrvToolkit = new toolkit(false);
        vrvToolkit.setResourcePath("../../../data");
        
        String options = "{'adjustPageHeight': true, 'breaks': 'auto', 'scale': 50}";
        
        /* set some options */
        vrvToolkit.setOptions(options);

		String mei = "";
        try {
            mei = new String(Files.readAllBytes(Paths.get("../../../doc/importer.mei")));
        } catch (IOException e) {
            System.out.println("Could not read the mei file: " + e.getMessage());
        }
        
        /* load the data */
		vrvToolkit.loadData(mei);

        /* render the data to svg  and write it */
        String svg;
		svg = vrvToolkit.renderToSVG();
        
        String path = "output.svg";
        try {
            Files.write( Paths.get(path), svg.getBytes());
        } catch (IOException e) {
            System.out.println("Could not write the svg file: " + e.getMessage());
        }
	}
}


