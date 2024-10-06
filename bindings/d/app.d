import std.stdio: writeln;
import verovio;

void main(string[] args)
{
    if (args.length == 1)
    {
        writeln("No file specified.");
        return;
    }
    
    auto toolkit = new VrvToolkit("data");

    toolkit.loadFile(args[1]);
    toolkit.select(`{"measureRange":"1-3"}`);
    toolkit.redoLayout();

    toolkit.renderToMIDIFile("sample.mid");
    toolkit.renderToSVGFile("sample.svg");
}
