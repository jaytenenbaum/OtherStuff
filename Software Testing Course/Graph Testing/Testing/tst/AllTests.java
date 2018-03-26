package tst;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

@RunWith(Suite.class)
@SuiteClasses({ AllDefTest.class, AllUseTest.class, EdgeCoverageTest.class,
		NodeCoverageTest.class, PrimePathTest.class })
public class AllTests {

}
