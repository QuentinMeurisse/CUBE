package util.debug;

import org.json.JSONArray;
import tiling.neighbourhood.ChangeColorMove;
import tiling.solution.UrbanBlockSolution;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Collectors;

public class JSONManager{

  public void write(JSONArray ja, String path){
    try(BufferedWriter bw = Files.newBufferedWriter(Paths.get(path))){
      ja.write(bw, 1, 2);
    }catch(IOException e){
      e.printStackTrace();
    }
  }


  public MoveDoubleLinkedList<ChangeColorMove> read(String path, UrbanBlockSolution sol){
    JSONArray ja = null;
    try(BufferedReader br = new BufferedReader(new FileReader(path))){
      String s = readAllLines(br);
      ja = new JSONArray(s);
    }catch(IOException e){
      e.printStackTrace();
    }

    MoveDoubleLinkedList<ChangeColorMove> toReturn = new MoveDoubleLinkedList<>(sol);
    for(int i = 0; i< ja.length(); i++){
      JSONArray a = (JSONArray) ja.get(i);
      int k = (int) a.get(0);
      int color = (int) a.get(1);
      int applied = (int)a.get(2);
      if(applied == 1)
        toReturn.add(new ChangeColorMove(k, color), true);
      else
        toReturn.add(toReturn.getLastMove(), false);
    }
    return toReturn;
  }

  public String readAllLines(BufferedReader br){
    return br.lines().collect(Collectors.joining());
  }




}
