/*
run mvn package
two times
then
javac -cp .:target/VerovioToolkit-1.0-SNAPSHOT.jar  main.java
java -cp .:target/VerovioToolkit-1.0-SNAPSHOT.jar  main
*/

import java.io.IOException;

import org.rismch.verovio.toolkit;
import org.rismch.verovio.convertFileFormat;
import org.rismch.verovio.NativeUtils;

public class main {
	public static void main(String argv[]) {
		try {
			NativeUtils.loadLibraryFromJar("/META-INF/lib/libverovio.jnilib");
		} catch (IOException e) {
			System.out.println("Could not load Verovio native lib: " + e.getMessage());
		}
		toolkit toolkit = new toolkit();

		String s;
		String out;

		s = "@start:00000400004625-1.1.1\n";
		s = s + "@clef:C-3\n";
		s = s + "@keysig:xFCG\n";
		s = s + "@key:\n";
		s = s + "@timesig:c\n";
		s = s + "@data:" + "'4CCDE" + "\n";
		s = s + "@end:00000400004625-1.1.1\n";
		
		
		toolkit.setFormat(convertFileFormat.paeFile);
		toolkit.loadString(s);

		out = toolkit.renderToSvg();
		
		System.out.println(out);

	}
}


