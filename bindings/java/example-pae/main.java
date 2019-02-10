import java.io.IOException;

/* import the toolkit */
import org.rismch.verovio.toolkit;
import org.rismch.verovio.fileFormat;
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

		String s;
		String out;

		s = "@start:00000400004625-1.1.1\n";
		s = s + "@clef:C-3\n";
		s = s + "@keysig:xFCG\n";
		s = s + "@key:\n";
		s = s + "@timesig:c\n";
		s = s + "@data:" + "'4CCDE" + "\n";
		s = s + "@end:00000400004625-1.1.1\n";
		
		/* set the format to PAE and load the data */
		vrvToolkit.setFormat(fileFormat.PAE);
		vrvToolkit.loadData(s);

        /* convert it and write it to the standard output */
		out = vrvToolkit.renderToSVG();
		System.out.println(out);
	}
}


