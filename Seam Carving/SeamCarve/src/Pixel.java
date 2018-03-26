
public class Pixel {
	double r;
	double g;
	double b;
	int originrow;
	int origincolumn;
	public Pixel(double r, double g, double b, int originrow, int origincolumn) {
		super();
		this.r = r;
		this.g = g;
		this.b = b;
		this.originrow = originrow;
		this.origincolumn = origincolumn;
	}
	@Override
	public String toString() {
		return "Pixel [r=" + r + ", g=" + g + ", b=" + b + ", originrow=" + originrow + ", origincolumn=" + origincolumn
				+ "]";
	}
	
}
