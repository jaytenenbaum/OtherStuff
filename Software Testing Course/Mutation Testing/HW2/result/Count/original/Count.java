// This is mutant program.
// Author : ysma

public class Count
{

    private int[] numbers;

    public Count( int[] numbers )
    {
        this.numbers = numbers;
    }

    public int count()
    {
        int count = 0;
        for (int i = 0; i < numbers.length; i++) {
            int x = numbers[i];
            if (x < 0) {
                if (x == -1 && i > 0 && numbers[i - 1] == -1) {
                    break;
                }
            } else {
                count++;
            }
        }
        return count;
    }

}
