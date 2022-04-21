package tiling.othercomponent;

import tiling.gui.SquarePanel;
import tiling.gui.TilingPanel;
import util.SquareTiling;
import util.Tiling;

public class ExportSquareListener extends ExportSearchListener{

  public ExportSquareListener(String imagePath, String tilingPath, int fileNumber){
    super(imagePath, tilingPath, fileNumber);
  }


  @Override
  protected SquarePanel createPanel(Tiling t){
    return new SquarePanel((SquareTiling) t);
  }
}
