package tst;

import static org.junit.Assert.*;

import org.junit.Test;

public class AllUseTest {

	@Test
	public void test() {
		assertEquals(1,Count.count(new int[]{1}), 0.01);
		assertEquals(0,Count.count(new int[]{-1,-1}), 0.01);
		assertEquals(0,Count.count(new int[]{}), 0.01);
		
	}

}
