package util;

import org.json.JSONArray;
import org.json.JSONObject;
import tiling.neighbourhood.ChangeColorMove;
import tiling.solution.BuildingInvariant;
import tiling.solution.UrbanBlockInvariant;
import tiling.solution.UrbanBlockSolution;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.function.Function;
import java.util.stream.Collectors;

public class ImportExport{

  public static void exportScreenShot(Component component, String fileName) throws Exception {
    BufferedImage image = new BufferedImage(component.getWidth(), component.getHeight(), BufferedImage.TYPE_INT_RGB);
    // paints into image's Graphics
    component.update(image.getGraphics());
    // write the captured image as a PNG
    ImageIO.write(image, "jpeg", new File(fileName));
  }

  public static void exportTilingToJSON(Tiling t, String path){
    JSONArray jsonArray = new JSONArray();
    for(int k = 0; k < t.numCells(); k++){
      Cell c = t.getCell(k);
      if(c.getColor() != 0){
        JSONArray ja = new JSONArray();
        ja.put(k);
        ja.put(c.getColor());
        ja.put(c.isLocked());
        jsonArray.put(ja);
      }
    }
    try(BufferedWriter bw = Files.newBufferedWriter(Paths.get(path))){
      jsonArray.write(bw, 1, 2);
    }
    catch(IOException e){
      e.printStackTrace();
    }
  }

  public static UrbanBlockSolution importTilingFromJson(String path,
                                                        int width, int height, double sideLength,
                                                        TriFunction<Integer, Integer, Double, Tiling> initTiling,
                                                        ArrayList<Function<UrbanBlockSolution, UrbanBlockInvariant>> invariants){

    UrbanBlockSolution toReturn = new UrbanBlockSolution(initTiling.apply(width, height, sideLength));
    for(Function<UrbanBlockSolution, UrbanBlockInvariant> f : invariants)
      f.apply(toReturn);
    JSONArray jsonArray = null;
    try(BufferedReader br = new BufferedReader(new FileReader(path))){
      String s = readAllLines(br);
      jsonArray = new JSONArray(s);
    }catch(IOException e){
      e.printStackTrace();
    }

    assert jsonArray != null;
    for(int i = 0; i < jsonArray.length(); i++){
      JSONArray ja = (JSONArray) jsonArray.get(i);
      int k = (int) ja.get(0);
      int color = (int) ja.get(1);
      boolean locked = (boolean) ja.get(2);
      if(locked)
        toReturn.lockCell(k, color);
      else{
        ChangeColorMove move = new ChangeColorMove(k, color);
        move.apply(toReturn);
      }
    }
    return toReturn;
  }

  private static String readAllLines(BufferedReader br){
    return br.lines().collect(Collectors.joining());
  }
}
