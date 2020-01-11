package z3.tree;

import java.io.File;

class InvalidPathException extends Exception
{
    public InvalidPathException()
    {
        super("InvalidPathException");
    }
}

public class Node
{
    String filename;
    long size;
    Node[] children;
    Node parent;
    File file;

    public Node(){
        this.size = 0;
    }

    public Node(Node Parent, String filepath) throws InvalidPathException
    {
        file = new File(filepath);
        if(!file.exists())
            throw new InvalidPathException();

        filename = file.getAbsolutePath();

        parent = Parent;

        if(representsFile() == false){
            children = new Node[file.list().length];
            createChildren();
            size = calculateSize();
        }
        else{
            size = file.length();
        }

    }

    public String getFilename()
    {
        return filename;
    }

    public long getSize()
    {
        return size;
    }

    public String getSizeHuman() {
        return ((double)size /(1024*1024)) + " MB";
    }

    public long calculateSize()
    {
        long f_size = 0;

        if(representsFile()){
            this.size = file.length();
            return this.size;
        }
        else{
            for(Node n : children)
                f_size += n.calculateSize();
            this.size = f_size;
        }
        return f_size;
    }

    public void createChildren() throws InvalidPathException
    {
        if(representsFile())
            return;

        File[] filelist;

        filelist = file.listFiles();

        for(int i = 0; i < filelist.length; i++){
            children[i] = new Node(this, filelist[i].getAbsolutePath());
        }

    }

    public boolean representsFile()
    {
        return !file.isDirectory();
    }

    public Node getMaxFileSize()
    {
        Node max = new Node();
        if(representsFile()){
            return this;
        }
        else{
            Node c_tmp = null;
            for(Node c: children){
                c_tmp = c.getMaxFileSize();
                if(c_tmp.size > max.size && c_tmp.representsFile())
                    max = c_tmp;
            }
        }
        return max;
    }

    public Node getMaxDirSize(Node root) {
        Node max = new Node();
        if (representsFile()) {
            return parent;
        } else {
            Node c_tmp = null;
            for (Node c : children) {
                c_tmp = c.getMaxDirSize(root);
                if (c_tmp.size > max.size && c_tmp.representsFile() == false && c_tmp != root)
                    max = c_tmp;
            }
        }
        return max;
    }

}
