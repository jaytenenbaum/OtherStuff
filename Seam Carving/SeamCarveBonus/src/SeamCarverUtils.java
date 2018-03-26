import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class SeamCarverUtils {
	/**
	 * output buffered image to a file
	 */
	public static void saveOutputImage(Pixel[][] outputImage, String outputFileLocation) {
		
		try {
			  BufferedImage bufferedImage = new BufferedImage(outputImage[0].length, outputImage.length, BufferedImage.TYPE_INT_RGB); 

			  for (int y = 0; y < outputImage.length; y++) {
			     for (int x = 0; x < outputImage[0].length; x++) {
			        int rgb = (int) outputImage[y][x].r;
			        rgb = (rgb << 8) + (int)outputImage[y][x].g; 
			        rgb = (rgb << 8) + (int)outputImage[y][x].b;
			        bufferedImage.setRGB(x, y, rgb);
			     }
			  }

			  File outputFile = new File(outputFileLocation);
			  ImageIO.write(bufferedImage, "png", outputFile);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * This method reads an image from the file
	 * @param inputFileLocation -- > eg. "C:/testImage.jpg"
	 * @return BufferedImage of the file read
	 */
	public static Pixel[][] getInputImage(String inputFileLocation) {
		BufferedImage img = null;
		
		try {
			img = ImageIO.read(new File(inputFileLocation));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		int width = img.getWidth();
		int height = img.getHeight();
		Pixel[][] inputImg = new Pixel[height][width];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				int rgb = img.getRGB(j, i);
				int red = (rgb >> 16) & 0xff;
				int green = (rgb >> 8) & 0xff;
				int blue = (rgb) & 0xff;
				inputImg[i][j] = new Pixel((double)red, (double)green, (double)blue, i, j);
				
			}
		}
		
		return inputImg;

	}
	
}
