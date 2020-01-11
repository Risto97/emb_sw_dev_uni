package z3.examples;

import z3.tree.Node;
import z3.tree.DirectoryTree;

public class Main
{

    public static void main(final String[] arguments)
    {
        if(arguments.length < 2){
            System.out.println("Input search path as argument");
            System.exit(-1);
        }

        DirectoryTree dt = new DirectoryTree(arguments[1]);

        System.out.println("MaxFile: " + dt.findMaxSizeFile().getFilename()+  " size: " + dt.findMaxSizeFile().getSize());
        System.out.println("MaxDir: " + dt.findMaxSizeDirectory().getFilename() + " size: "
        + dt.findMaxSizeDirectory().getSize());
    }

}
