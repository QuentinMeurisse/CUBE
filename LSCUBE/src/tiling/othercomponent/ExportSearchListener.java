package tiling.othercomponent;

import org.jamesframework.core.search.Search;
import org.jamesframework.core.search.listeners.SearchListener;
import tiling.gui.TilingPanel;
import tiling.solution.UrbanBlockSolution;
import util.ImportExport;
import util.Tiling;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowEvent;

public abstract class ExportSearchListener implements SearchListener<UrbanBlockSolution>{

  private final String imagePath, tilingPath;
  private final int fileNumber;

 public ExportSearchListener(String imagePath, String tilingPath, int fileNumber){
    this.imagePath = imagePath;
    this.tilingPath = tilingPath;
    this.fileNumber = fileNumber;
  }

  protected abstract TilingPanel createPanel(Tiling t);

  @Override
  public void searchStopped(Search search){
    UrbanBlockSolution best = (UrbanBlockSolution) search.getBestSolution();
    TilingPanel panel = createPanel(best.getGrid());
    panel.setPreferredSize(new Dimension(680, 680));
    panel.setSize(panel.getPreferredSize());

    String imageFile = imagePath + "/tiling" + fileNumber + ".jpeg";
    String jsonFile = tilingPath + "/tiling" + fileNumber + ".json";

    JFrame frame = new JFrame();
    frame.add(panel);
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    frame.pack();
    frame.setVisible(true);

    try{
      ImportExport.exportScreenShot(panel, imageFile);
    }catch(Exception e){
      e.printStackTrace();
    }

    frame.setVisible(false);
    frame.dispose();
    ImportExport.exportTilingToJSON(best.getGrid(), jsonFile);

    //frame.dispatchEvent(new WindowEvent(frame, WindowEvent.WINDOW_CLOSING));


  }
}
