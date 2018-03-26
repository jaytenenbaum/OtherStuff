/**
 * @author Jay Tenenbaum Alon Goldberg
 */
public class SeamCarver {
	String inputFilename;
	Pixel[][] inputImage;
	int columns;
	int rows;
	int energyType;
	String outputFilename;
	Pixel[][] outputImage;
	// matrix with the energy of the image
	double[][] energyMatrix;
	// matrix used to enlarge the photo. contains zeros and ones.
	int[][] enlargementMatrix;

	public static void main(String[] args) {
		// build seam carver
		SeamCarver sc = new SeamCarver();
		sc.inputFilename = args[0];
		sc.columns = Integer.parseInt(args[1]);
		sc.rows = Integer.parseInt(args[2]);
		sc.energyType = Integer.parseInt(args[3]);
		sc.outputFilename = args[4];
		sc.seamCarve();
	}

	/**
	 * calculates the output, and saves it
	 */
	private void seamCarve() {
		inputImage = SeamCarverUtils.getInputImage(inputFilename);
		outputImage = carveOutputImageBonus();
		SeamCarverUtils.saveOutputImage(outputImage, outputFilename);

	}

	/**
	 * calculate and return the output image
	 */
	private Pixel[][] carveOutputImage() {
		Pixel[][] img = getClonedImage(inputImage);
		// get to the right amount of columns
		if (columns < inputImage[0].length) {// shrink
			for (int i = inputImage[0].length; i > columns; i--) {
				img = removeVerticalSeam(img, false);
//				System.out.println("#cols = " + img[0].length);
			}
		} else {// enlarge
			enlargementMatrix = getZeroEnlargementMatrix(img);
			calcOriginRowsAndCols(img);
			for (int i = inputImage[0].length; i < columns; i++) {
				// remove vertical seam in dynamic picture and remember it in
				// enlargement matrix
				img = removeVerticalSeam(img, true);
//				System.out.println("#cols = " + (i + 1));
			}
			img = enlarge(inputImage, columns);
		}

		// get to the right amount of rows
		Pixel[][] transposeImage = getClonedImage(transpose(img));
		if (rows < transposeImage[0].length) {// shrink
			for (int i = transposeImage[0].length; i > rows; i--) {
				transposeImage = removeVerticalSeam(transposeImage, false);
//				System.out.println("#rows = " + transposeImage[0].length);
			}
			img = transpose(transposeImage);
		} else {// enlarge
			enlargementMatrix = getZeroEnlargementMatrix(transposeImage);
			calcOriginRowsAndCols(transposeImage);
			for (int i = transposeImage[0].length; i < rows; i++) {
				// remove vertical seam in dynamic picture and remember it in
				// enlargement matrix
				transposeImage = removeVerticalSeam(transposeImage, true);
//				System.out.println("#rows = " + (i + 1));
			}
			transposeImage = enlarge(transpose(img), rows);
			img = transpose(transposeImage);
		}

		return img;
	}
	
	
	private Pixel[][] carveOutputImageBonus()
	{
		Pixel[][] img = getClonedImage(inputImage);		
		
		if (columns < inputImage[0].length && rows < inputImage.length) 
		{
			ImageAndValue [][] dynamicMatrix = new ImageAndValue[inputImage.length - rows + 1][inputImage[0].length - columns + 1];
			for	(int i=0; i < dynamicMatrix.length; i++)
			{
				for (int j=0; j < dynamicMatrix[0].length; j++)
				{
					if (i==0 && j==0)
					{
						dynamicMatrix[0][0] = new ImageAndValue(0, getClonedImage(img));
						continue;
					}
					if (i==0)
					{
						Pixel [][] fromLeftImg = getClonedImage(dynamicMatrix[i][j-1].img);
						ImageAndValue fromLeft = new ImageAndValue(dynamicMatrix[i][j-1].val + removeVerticalSeamBonus(fromLeftImg, false),fromLeftImg);
						dynamicMatrix[i][j] = fromLeft;
						continue;
					}
					if (j==0)
					{
						Pixel [][] fromUpImg = transpose(getClonedImage(dynamicMatrix[i-1][j].img));
						ImageAndValue fromUp = new ImageAndValue(dynamicMatrix[i-1][j].val + removeVerticalSeamBonus(fromUpImg, false),fromUpImg);
						fromUp.img  = transpose(fromUp.img);
						dynamicMatrix[i][j] = fromUp;
						dynamicMatrix[i-1][j].img = null;
						continue;
					}
					
					Pixel [][] fromLeftImg = getClonedImage(dynamicMatrix[i][j-1].img);
					ImageAndValue fromLeft = new ImageAndValue(dynamicMatrix[i][j-1].val + removeVerticalSeamBonus(fromLeftImg, false),fromLeftImg);
					Pixel [][] fromUpImg = transpose(getClonedImage(dynamicMatrix[i-1][j].img));
					ImageAndValue fromUp = new ImageAndValue(dynamicMatrix[i-1][j].val + removeVerticalSeamBonus(fromUpImg, false),fromUpImg);
					fromUp.img  = transpose(fromUp.img);
					dynamicMatrix[i-1][j].img = null;
					
					if (fromUp.val < fromLeft.val)
						dynamicMatrix[i][j] = fromUp;
					else
						dynamicMatrix[i][j] = fromLeft;
				}
			}
			

			int columnIndex = dynamicMatrix[0].length -1;
			int rowIndex = dynamicMatrix.length -1;
			int path [] = new int[dynamicMatrix.length + dynamicMatrix[0].length];
			for	(int i = dynamicMatrix.length + dynamicMatrix[0].length - 2; i>=0; i--)
			{
				if (columnIndex == 0)
				{
					path[i] = 0;
					continue;
				}
				if (rowIndex == 0)
				{
					path[i] = 1;
					continue;
				}
				
				if (dynamicMatrix[rowIndex][columnIndex-1].val < dynamicMatrix[rowIndex-1][columnIndex].val)
				{
					columnIndex--;
					path[i] = 1;
				}
				else
				{
					rowIndex--;
					path[i] = 0;
				}
			}
			
			for (int i=0; i<path.length; i++)
			{
				if (path[i] == 1)
					img = removeVerticalSeam(img, false);
				else
				{
					img = transpose(removeVerticalSeam(transpose(img), false));
				}
			}
		}
		else
			return carveOutputImage();
		
		return img;
	}

	private double removeVerticalSeamBonus(Pixel[][] img, boolean markPathForEnlargement)
	{
		int m = img.length;
        	int n = img[0].length;
        
		double[][] energyImage = getEnergyImage(img);
		
		double[][] dynamicMatrix;
		
		if (this.energyType != 2)
			dynamicMatrix = getDynamicMatrix(energyImage);
		else
			dynamicMatrix = getForwardEnergyDynamicMatrix(img);
		
		int seamIndex = findLowestEnergySeamColumnIndex(dynamicMatrix);
		
		int[] path = findSeamPath(dynamicMatrix, seamIndex);
		
//		if(path.length != img.length){
//			System.out.println("path of lenght"+path.length+"should be"+img.length);
//		}
		if(markPathForEnlargement){
			markPath(path, img);
		}
		Pixel[][] outputImg = new Pixel[m][n - 1];
		
		/**
		 * set output image to be img without the path indices (same height but one less width)
		 */
		for (int i = 0; i < m; i++)
        	{
        		for (int originalImageJ = 0, outputImageJ = 0; originalImageJ < n; originalImageJ++, outputImageJ++)
        		{ 
        			if (originalImageJ != path[i])
        				outputImg[i][outputImageJ] = img[i][originalImageJ];
        			else
        				outputImageJ--;
        		}
       		}
		
		img =  outputImg;
		return dynamicMatrix[m-1][seamIndex];
	}


	/**
	 * enlarge the photo to give it "size" columns based on enlargement matrix
	 * 
	 * @param img
	 * @param size
	 * @return
	 */
	private Pixel[][] enlarge(Pixel[][] img, int size) {
		Pixel[][] outputImage = new Pixel[img.length][size];
		int outputj = 0;
		for (int i = 0; i < img.length; i++) {
			outputj = 0;
			for (int j = 0; j < img[0].length; j++) {
				if (enlargementMatrix[i][j] == 0) {
					Pixel cur = img[i][j];
					outputImage[i][outputj] = new Pixel(cur.r, cur.g, cur.b, i, outputj);
					outputj++;
				}
				if (enlargementMatrix[i][j] == 1) {
					Pixel cur = img[i][j];
					// copy two pixels
					outputImage[i][outputj] = new Pixel(cur.r, cur.g, cur.b, i, outputj);
					outputj++;
					outputImage[i][outputj] = new Pixel(cur.r, cur.g, cur.b, i, outputj);
					outputj++;
				}

			}
		}
		return outputImage;
	}

	/**
	 * give each pixel in the image a (i,j) location
	 * 
	 * @param img
	 */
	private void calcOriginRowsAndCols(Pixel[][] img) {
		for (int i = 0; i < img.length; i++) {
			for (int j = 0; j < img[0].length; j++) {
				img[i][j].originrow = i;
				img[i][j].origincolumn = j;
			}
		}
	}

	/**
	 * clone the given image
	 * 
	 * @param img
	 * @return
	 */
	private Pixel[][] getClonedImage(Pixel[][] img) {
		Pixel[][] ans = new Pixel[img.length][img[0].length];
		for (int i = 0; i < img.length; i++) {
			for (int j = 0; j < img[0].length; j++) {
				Pixel cur = img[i][j];
				ans[i][j] = new Pixel(cur.r, cur.g, cur.b, cur.originrow, cur.origincolumn);
			}
		}
		return ans;
	}

	/**
	 * get an enlargement matrix in image's size, for enlargement
	 * 
	 * @param img
	 * @return
	 */
	private int[][] getZeroEnlargementMatrix(Pixel[][] img) {
		int[][] ans = new int[img.length][img[0].length];
		for (int i = 0; i < img.length; i++) {
			for (int j = 0; j < img[0].length; j++) {
				ans[i][j] = 0;
			}
		}
		return ans;
	}

	/**
	 * return the image removed the smallest vertical seam.
	 * markPathForEnlargement is true iff we are enlarging
	 * 
	 * @param img
	 * @param markPathForEnlargement
	 * @return
	 */
	private Pixel[][] removeVerticalSeam(Pixel[][] img, boolean markPathForEnlargement) {
		int m = img.length;
		int n = img[0].length;

		double[][] energyImage = getEnergyImage(img);

		double[][] dynamicMatrix;

		if (this.energyType != 2)
			dynamicMatrix = getDynamicMatrix(energyImage);
		else
			dynamicMatrix = getForwardEnergyDynamicMatrix(img);

		int[] path = findLowestEnergySeamPath(dynamicMatrix);
//		if (path.length != img.length) {
//			System.out.println("path of lenght" + path.length + "should be" + img.length);
//		}
		// for enlarging, mark path
		if (markPathForEnlargement) {
			markPath(path, img);
		}
		Pixel[][] outputImg = new Pixel[m][n - 1];

		/**
		 * set output image to be img without the path indices (same height but
		 * one less width)
		 */
		for (int i = 0; i < m; i++) {
			for (int originalImageJ = 0, outputImageJ = 0; originalImageJ < n; originalImageJ++, outputImageJ++) {
				if (originalImageJ != path[i])
					outputImg[i][outputImageJ] = img[i][originalImageJ];
				else
					outputImageJ--;
			}
		}

		return outputImg;
	}

	/**
	 * mark a given path on the enlargement matrix
	 * 
	 * @param path
	 * @param img
	 */
	private void markPath(int[] path, Pixel[][] img) {
		for (int i = 0; i < path.length; i++) {
			Pixel wantedPixel = img[i][path[i]];
			enlargementMatrix[wantedPixel.originrow][wantedPixel.origincolumn] += 1;
		}
	}

	/**
	 * get the path (indices of width) of the minimal path
	 * 
	 * @param dynamicMatrix
	 * @return
	 */
	private int[] findLowestEnergySeamPath(double[][] dynamicMatrix) {
		int seamIndex = findLowestEnergySeamColumnIndex(dynamicMatrix);
		return findSeamPath(dynamicMatrix, seamIndex);
	}

	/**
	 * get the best path given the last seam index, and the dynamic matrix (this
	 * is the backtracking)
	 * 
	 * @param dynamicMatrix
	 * @param seamIndex
	 * @return
	 */
	private int[] findSeamPath(double[][] dynamicMatrix, int seamIndex) {
		int m = dynamicMatrix.length;
		int[] path = new int[m];
		for (int i = m - 1; i > 0; i--) {
			path[i] = seamIndex;
			seamIndex = getPreviousSeamIndex(dynamicMatrix, seamIndex, i);
		}
		path[0] = seamIndex;
		return path;
	}

	/**
	 * get the previous index of the seam on the path, given the current one,
	 * the row (i), and the dynamic matrix
	 * 
	 * @param dynamicMatrix
	 * @param seamIndex
	 * @param i
	 * @return
	 */
	private int getPreviousSeamIndex(double[][] dynamicMatrix, int seamIndex, int i) {
		// on rightest column
		if (seamIndex == dynamicMatrix[0].length - 1) {
			// return best of upper and upper-left
			if (dynamicMatrix[i - 1][seamIndex - 1] > dynamicMatrix[i - 1][seamIndex]) {
				return seamIndex - 1;
			}
			return seamIndex;
		}

		// on leftest column
		if (seamIndex == 0) {
			// return best of upper and upper-left
			if (dynamicMatrix[i - 1][1] > dynamicMatrix[i - 1][0]) {
				return 1;
			}
			return 0;
		}

		// general solution. take min of 3.
		double minPrevValue = Math.min(dynamicMatrix[i - 1][seamIndex - 1],
				Math.min(dynamicMatrix[i - 1][seamIndex], dynamicMatrix[i - 1][seamIndex + 1]));
		if (dynamicMatrix[i - 1][seamIndex - 1] == minPrevValue) {
			return seamIndex - 1;
		} else if (dynamicMatrix[i - 1][seamIndex] == minPrevValue) {
			return seamIndex;
		} else {
			return seamIndex + 1;
		}
	}

	/**
	 * returns the index of the pixel in the last row with smallest value in
	 * dynamic matrix (smallest cumulative energy through path)
	 * 
	 * @param dynamicMatrix
	 * @return
	 */
	private int findLowestEnergySeamColumnIndex(double[][] dynamicMatrix) {
		int m = dynamicMatrix.length;
		int n = dynamicMatrix[0].length;

		int index = 0;
		double min = Double.MAX_VALUE;
		for (int j = 0; j < n; j++) {
			double dynamicMatrixValue = dynamicMatrix[m - 1][j];
			if (dynamicMatrixValue < min) {
				min = dynamicMatrixValue;
				index = j;
			}
		}
		return index;
	}

	/**
	 * get the cumulative energy matrix (which we seek for minimum using dynamic
	 * programming)
	 * 
	 * @param energyImage
	 * @return
	 */
	private double[][] getDynamicMatrix(double[][] energyImage) {
		int m = energyImage.length;
		int n = energyImage[0].length;

		double[][] dynamicMatrix = new double[m][n];
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (i == 0)
					dynamicMatrix[i][j] = energyImage[i][j];
				else if (j == 0)// take result based on upper and upper right
					// pixels
					dynamicMatrix[i][j] = energyImage[i][j]
							+ Math.min(dynamicMatrix[i - 1][j], dynamicMatrix[i - 1][j + 1]);
				else if (j == n - 1)// take result based on upper and upper left
					// pixels
					dynamicMatrix[i][j] = energyImage[i][j]
							+ Math.min(dynamicMatrix[i - 1][j], dynamicMatrix[i - 1][j - 1]);
				else// take result based on 3 upper pixels
				{
					dynamicMatrix[i][j] = energyImage[i][j]
							+ Math.min(Math.min(dynamicMatrix[i - 1][j - 1], dynamicMatrix[i - 1][j]),
									dynamicMatrix[i - 1][j + 1]);
				}
			}
		}
		return dynamicMatrix;
	}

	/**
	 * calculate the energy of the input image
	 * 
	 * @param img
	 * @return
	 */
	private double[][] getEnergyImage(Pixel[][] img) {
		int m = img.length;
		int n = img[0].length;
		// premake the pmn matrix if we use entropy
		double[][] pmnLogPmnMatrix = null;
		if (energyType == 1) {
			double[][] grayScaleImage = grayscale(img);
			pmnLogPmnMatrix = pmnLogPmnMatrix(grayScaleImage);
		}
		double[][] energyImage = new double[m][n];
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (energyType == 0) {// take only energy
					energyImage[i][j] = computePureEnergyAtPixel(img, i, j, m, n);
				}
				if (energyType == 1) {// add entropy with energy

					energyImage[i][j] = (computePureEnergyAtPixel(img, i, j, m, n)
							+ computeEntropyAtPixel(pmnLogPmnMatrix, i, j, m, n)) / 2;
				}
			}
		}
		return energyImage;
	}

	/**
	 * compute the entropy at given pixel (i,j)
	 */
	private double computeEntropyAtPixel(double[][] pmnLogPmnMatrix, int i, int j, int m, int n) {
		double entropy = 0;
		int numOfNeighbors = 1;
		// calculate entropy
		for (int row = -4; row < 5; row++) {
			for (int column = -4; column < 5; column++) {
				// Make sure the neighbor is in the image
				if (i + row < m && i + row >= 0 && j + column < n && j + column >= 0) {
					numOfNeighbors++;
					entropy -= pmnLogPmnMatrix[i + row][j + column];
				}
			}
		}

		return entropy / numOfNeighbors;
	}

	/**
	 * create the matrix of the pmnlogpmn values
	 * 
	 * @param grayScaleImage
	 * @return
	 */
	private double[][] pmnLogPmnMatrix(double[][] grayScaleImage) {
		double[][] pmnLogPmnMatrix = new double[grayScaleImage.length][grayScaleImage[0].length];
		double pmn;
		for (int i = 0; i < grayScaleImage.length; i++) {
			for (int j = 0; j < grayScaleImage[0].length; j++) {
				pmn = grayScaleImage[i][j] / averageOfGrayscalesAround(grayScaleImage, i, j, grayScaleImage.length,
						grayScaleImage[0].length);
				pmnLogPmnMatrix[i][j] = pmn * Math.log(pmn);
			}
		}
		return pmnLogPmnMatrix;
	}

	/**
	 * get the average of grayscales of neighbors
	 */
	private double averageOfGrayscalesAround(double[][] grayScaleImage, int i, int j, int m, int n) {
		int numOfNeighbors = 0;
		double sumOfGrayscales = 0;
		for (int row = -4; row < 5; row++) {
			for (int column = -4; column < 5; column++) {
				// Make sure the neighbor is in the image
				if (i + row < m && i + row >= 0 && j + column < n && j + column >= 0) {
					numOfNeighbors++;
					sumOfGrayscales += grayScaleImage[i + row][j + column];
				}
			}
		}

		return sumOfGrayscales / numOfNeighbors;
	}

	/**
	 * return grayscale of given image
	 * 
	 * @param img
	 * @return
	 */
	private double[][] grayscale(Pixel[][] img) {
		double[][] grayScaleImage = new double[img.length][img[0].length];
		for (int i = 0; i < img.length; i++) {
			for (int j = 0; j < img[0].length; j++) {
				Pixel pixel = img[i][j];
				grayScaleImage[i][j] = (pixel.r + pixel.g + pixel.b) / 3;
			}
		}
		return grayScaleImage;
	}

	/**
	 * compute the pure energy (no entropy) at given pixel
	 */
	private double computePureEnergyAtPixel(Pixel[][] img, int i, int j, int m, int n) {
		double energy = 0;
		// Number of neighbors
		int numOfNeighbors = 0;
		double sum = 0;
		for (int row = -1; row < 2; row++) {
			for (int column = -1; column < 2; column++) {
				// Make sure the neighbor is in the image
				if (i + row < m && i + row >= 0 && j + column < n && j + column >= 0 && !(row == 0 && column == 0)) {
					numOfNeighbors++;
					sum += pixelsDifference(img, i + row, j + column, i, j);
				}
			}
		}
		// Normalize the sum according to the number of neighbors
		energy = sum / numOfNeighbors;
		return energy;
	}

	/**
	 * return the matrix for forward energy
	 * 
	 * @param img
	 * @return
	 */
	private double[][] getForwardEnergyDynamicMatrix(Pixel[][] img) {
		int m = img.length;
		int n = img[0].length;

		double[][] dynamicMatrix = new double[m][n];
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (i == 0)
					dynamicMatrix[i][j] = computeForwardEnergyAtPixel(img, i, j, 0, n);
				else if (j == 0)
					dynamicMatrix[i][j] = Math.min(
							dynamicMatrix[i - 1][j] + computeForwardEnergyAtPixel(img, i, j, 0, n),
							dynamicMatrix[i - 1][j + 1] + computeForwardEnergyAtPixel(img, i, j, 1, n));
				else if (j == n - 1)
					dynamicMatrix[i][j] = Math.min(
							dynamicMatrix[i - 1][j] + computeForwardEnergyAtPixel(img, i, j, 0, n),
							dynamicMatrix[i - 1][j - 1] + computeForwardEnergyAtPixel(img, i, j, -1, n));
				else {
					dynamicMatrix[i][j] = Math.min(
							Math.min(dynamicMatrix[i - 1][j - 1] + computeForwardEnergyAtPixel(img, i, j, -1, n),
									dynamicMatrix[i - 1][j] + computeForwardEnergyAtPixel(img, i, j, 0, n)),
							dynamicMatrix[i - 1][j + 1] + computeForwardEnergyAtPixel(img, i, j, 1, n));
				}
			}
		}
		return dynamicMatrix;
	}

	/**
	 * get the forward energy at a given pixel
	 */
	private double computeForwardEnergyAtPixel(Pixel[][] img, int i, int j, int upperPixelJIndexShift, int n) {
		double forwardEnergy;

		if (j + 1 >= n)
			forwardEnergy = pixelsDifference(img, i, j - 1, i, j);
		else if (j - 1 < 0)
			forwardEnergy = pixelsDifference(img, i, j, i, j + 1);
		else
			forwardEnergy = pixelsDifference(img, i, j - 1, i, j + 1);

		if (upperPixelJIndexShift == -1) {
			forwardEnergy += pixelsDifference(img, i - 1, j, i, j - 1);
		}
		if (upperPixelJIndexShift == 1) {
			forwardEnergy += pixelsDifference(img, i - 1, j, i, j + 1);
		}

		return forwardEnergy;
	}

	/**
	 * calculate the difference in rgb between pixels
	 */
	private double pixelsDifference(Pixel[][] img, int firstPixelI, int firstPixelJ, int secondPixelI,
			int secondPixelJ) {
		return (Math.abs(img[firstPixelI][firstPixelJ].r - img[secondPixelI][secondPixelJ].r)
				+ Math.abs(img[firstPixelI][firstPixelJ].g - img[secondPixelI][secondPixelJ].g)
				+ Math.abs(img[firstPixelI][firstPixelJ].b - img[secondPixelI][secondPixelJ].b)) / 3;
	}

	/**
	 * get the matrix transposed
	 * 
	 * @param img
	 * @return
	 */
	public Pixel[][] transpose(Pixel[][] img) {
		int m = img.length;
		int n = img[0].length;
		Pixel[][] transposeImage = new Pixel[n][m];
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				transposeImage[j][i] = img[i][j];
		return transposeImage;
	}
}
