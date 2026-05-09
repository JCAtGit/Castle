import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

import ghidra.app.script.GhidraScript;
import ghidra.app.util.Option;
import ghidra.app.util.exporter.CppExporter;

public class ghidra_postscript extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        File outputFile = new File(args[0]);

        CppExporter exporter = new CppExporter();
        List<Option> opts = new ArrayList<>();
        opts.add(new Option(CppExporter.CREATE_HEADER_FILE, false));
        exporter.setOptions(opts);
        exporter.setExporterServiceProvider(state.getTool());
        exporter.export(outputFile, currentProgram, null, monitor);
    }
}
