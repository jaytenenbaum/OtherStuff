// This is mutant program.
// Author : ysma

public class gFuncClass
{

    public static double g( double x )
    {
        return sqr( abs( x ) ) * 2;
    }

    public static double sqr( double x )
    {
        return x * x;
    }

    public static double sqrt( double x )
    {
        return Math.sqrt( -x );
    }

    public static double abs( double x )
    {
        return Math.abs( x );
    }

}
