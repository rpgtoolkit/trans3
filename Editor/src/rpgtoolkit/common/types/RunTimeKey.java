package rpgtoolkit.common.types;

public class RunTimeKey
{
    private int key;
    private String program;

    public RunTimeKey(int key, String program)
    {
        this.key = key;
        this.program = program;
    }

    public int getKey()
    {
        return key;
    }

    public String getProgram()
    {
        return program;
    }

    public void setKey(int key)
    {
        this.key = key;
    }

    public void setProgram(String program)
    {
        this.program = program;
    }

}
