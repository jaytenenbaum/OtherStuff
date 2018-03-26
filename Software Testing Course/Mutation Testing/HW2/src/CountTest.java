import junit.framework.TestCase;


public class CountTest extends TestCase {
	public void test1() {
		Count c = new Count(new int[]{1,-1,-1});
		assertEquals("Counted value", c.count(), 1);
	}

	public void test2() {
		Count c = new Count(new int[]{1});
		assertEquals("Counted value", c.count(), 1);
	}

	public void test3() {
		Count c = new Count(new int[]{-1, 1});
		assertEquals("Counted value", c.count(), 1);
	}

	public void test4() {
		Count c = new Count(new int[]{1, 2});
		assertEquals("Counted value", c.count(), 2);
	}

	public void test5() {
		Count c = new Count(new int[]{});
		assertEquals("Counted value", c.count(), 0);
	}
	
	
}
