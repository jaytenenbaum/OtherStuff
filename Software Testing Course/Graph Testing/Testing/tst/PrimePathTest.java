package tst;

import static org.junit.Assert.*;

import org.junit.Test;

public class PrimePathTest {

	@Test
	public void test() {
		assertEquals(0,Count.count(new int[]{-1,-1}), 0.01);
		assertEquals(0,Count.count(new int[]{}), 0.01);
		assertEquals(1,Count.count(new int[]{1,-1,-1}), 0.01);
		assertEquals(2,Count.count(new int[]{3,2}), 0.01);
		assertEquals(1,Count.count(new int[]{-5,3}), 0.01);
		assertEquals(0,Count.count(new int[]{-2}), 0.01);
	}

}
