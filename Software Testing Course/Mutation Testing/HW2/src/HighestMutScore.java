/*
 * ###############################################################################################
 * #################################	Maximum Mutation Score: 91 ###############################
 * ###############################################################################################
 */


import junit.framework.TestCase;
public class HighestMutScore extends TestCase{
	public void test1() {
		Count c = new Count(new int[]{1,-1,-1});
		assertEquals("Counted value", c.count(), 1);
	}

	public void test2() {
		Count c = new Count(new int[]{-1, 1});
		assertEquals("Counted value", c.count(), 1);
	}

	public void test3() {
		Count c = new Count(new int[]{-3,-2,-1,0,0,1,-1});
		assertEquals("Counted value", c.count(), 3);
	}
	
	public void test4() {
		Count c = new Count(new int[]{-3,-1,-1,0,1,-1});
		assertEquals("Counted value", c.count(), 0);
	}
	
	public void test5() {
		Count c = new Count(new int[]{0,-1,0,0,1,-1});
		assertEquals("Counted value", c.count(), 4);
	}
	
	public void test6() {
		Count c = new Count(new int[]{-1,-3,0,0,1,-1});
		new Count(new int[]{});
		assertEquals("Counted value", c.count(), 3);
	}
}
