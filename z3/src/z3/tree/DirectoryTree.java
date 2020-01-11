package z3.tree;

import java.io.File;

public class DirectoryTree
{
    Node root;

    public DirectoryTree(String filepath)
    {
        try{
            root = new Node(null, filepath);
        }
        catch(InvalidPathException e)
            {
                System.out.println(e.getMessage());
                System.exit(-1);
            }
    }

    public Node findMaxSizeFile()
    {
        return root.getMaxFileSize();
    }

    public Node findMaxSizeDirectory()
    {
        return root.getMaxDirSize(root);
    }
}
