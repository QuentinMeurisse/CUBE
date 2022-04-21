package tiling.gui;

//import sun.misc.JavaLangAccess;
import util.Tiling;

import javax.swing.*;
import java.awt.*;

public class TilingFrame extends JFrame{

  private JLabel evaluationValue = new JLabel("0");
  private JLabel perimeterValue = new JLabel("0");
  private JLabel areaValue = new JLabel("0");
  private JLabel numBuildingValue = new JLabel("0");
  private JLabel greenSpaceAreaValue = new JLabel("0");
  private TilingPanel grid;

  public TilingFrame(TilingPanel tp){
    super("Output");
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    setSize(1920, 1080);
    this.grid = tp;
    setLayout(new BorderLayout());

    JLabel evaluation = new JLabel("Evaluation: ");
    evaluation.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    JLabel perimeter = new JLabel("Buildings total perimeter: ");
    perimeter.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    JLabel area = new JLabel("Buildings total area: ");
    area.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    JLabel numBuilding = new JLabel("Number building:");
    numBuilding.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    JLabel greenSpacesArea = new JLabel("Green spaces area %: ");
    greenSpacesArea.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16));

    evaluationValue.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    perimeterValue.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    areaValue.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    numBuildingValue.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    greenSpaceAreaValue.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16));

    JPanel labelPanel = new JPanel();
    labelPanel.setLayout(new GridBagLayout());
    GridBagConstraints gbc = new GridBagConstraints();
    gbc.insets = new Insets(0, 0, 10, 10);
    gbc.anchor = GridBagConstraints.FIRST_LINE_END;
    gbc.gridy = 0;
    gbc.gridx = 0;
    labelPanel.add(evaluation, gbc);
    gbc.gridy = 1;
    gbc.gridx = 0;
    labelPanel.add(area, gbc);
    gbc.gridy = 2;
    gbc.gridx = 0;
    labelPanel.add(perimeter, gbc);
    gbc.gridy = 3;
    gbc.gridx = 0;
    labelPanel.add(numBuilding, gbc);
    gbc.gridy = 4;
    gbc.gridx = 0;
    labelPanel.add(greenSpacesArea, gbc);
    gbc.gridy = 0;
    gbc.gridx = 1;
    gbc.anchor = GridBagConstraints.FIRST_LINE_START;
    labelPanel.add(evaluationValue, gbc);
    gbc.gridy = 1;
    gbc.gridx = 1;
    labelPanel.add(areaValue, gbc);
    gbc.gridy = 2;
    gbc.gridx = 1;
    labelPanel.add(perimeterValue, gbc);
    gbc.gridy = 3;
    gbc.gridx = 1;
    labelPanel.add(numBuildingValue, gbc);
    gbc.gridy = 4;
    gbc.gridx = 1;
    labelPanel.add(greenSpaceAreaValue, gbc);

    JScrollPane scrollPane = new JScrollPane(labelPanel, ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
            ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
    scrollPane.setMaximumSize(new Dimension(550, 300));
    scrollPane.setPreferredSize(new Dimension(550, 300));

    scrollPane.setViewportView(labelPanel);
    scrollPane.setBorder(BorderFactory.createEmptyBorder());

    JPanel east = new JPanel();
    east.setLayout(new GridLayout(5, 1));
    east.add(scrollPane);

    add(grid, BorderLayout.CENTER);
    add(east, BorderLayout.EAST);

    GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
    GraphicsDevice[] gd = ge.getScreenDevices();
    if(gd.length > 1)
      this.setLocation(getX(), gd[1].getDefaultConfiguration().getBounds().y +100);


    setVisible(true);

  }

  public void setEvaluation(String newText){
    evaluationValue.setText(newText);
  }

  public void setArea(String newText){
    areaValue.setText(newText);
  }

  public void setPerimeter(String newText){
    perimeterValue.setText(newText);
  }

  public void setNumBuilding(String newText){
    numBuildingValue.setText(newText);
  }

  public void setGreenSpaceArea(String newText){
    greenSpaceAreaValue.setText(newText);
  }


  public void finished(){
    evaluationValue.setForeground(Color.GREEN);
    areaValue.setForeground(Color.GREEN);
    perimeterValue.setForeground(Color.GREEN);
    numBuildingValue.setForeground(Color.GREEN);
    greenSpaceAreaValue.setForeground(Color.GREEN);
  }

  public void setGrid(Tiling t){
    grid.setTiling(t);
    repaint();
  }
}
