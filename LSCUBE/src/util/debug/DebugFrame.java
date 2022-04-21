package util.debug;

import tiling.gui.TilingPanel;
import tiling.neighbourhood.ChangeColorMove;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ListIterator;
import java.util.concurrent.TimeUnit;

public class DebugFrame extends JFrame{

  public DebugFrame(TilingPanel tp, ListIterator<ChangeColorMove> iterator){
    super("Debug");

    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    setSize(500, 400);
    setLayout(new BorderLayout());
    add(tp, BorderLayout.CENTER);

    JLabel index = new JLabel("Current index: ");
    add(index, BorderLayout.NORTH);

    JPanel buttonPanel = new JPanel();

    JButton prevButton = new JButton("PREV");
    prevButton.addMouseListener(new MouseAdapter(){
      private boolean mousePressed = false;
      @Override
      public void mousePressed(MouseEvent mouseEvent){
        super.mousePressed(mouseEvent);
        mousePressed = true;
        new Thread(() ->{
          while(mousePressed){
            if(iterator.hasPrevious()){
              index.setText("current index: " + iterator.previousIndex());
              MoveDoubleLinkedList.DoubleLinkedListIterator dlli =
                      (MoveDoubleLinkedList.DoubleLinkedListIterator) iterator;
              String msg = dlli.isPreviousApplied() ? "applied" : "undo";
              //System.out.print(msg + " move: ");

              ChangeColorMove move = iterator.previous();

              //System.out.println(move);

              repaint();
            }
            try{
              TimeUnit.MILLISECONDS.sleep(200);
            }catch(InterruptedException e){
              e.printStackTrace();
            }
          }
        }).start();
      }

      @Override
      public void mouseReleased(MouseEvent mouseEvent){
        super.mouseReleased(mouseEvent);
        mousePressed = false;
      }
    });

    JButton nextButton = new JButton("NEXT");
    nextButton.addMouseListener(new MouseAdapter(){
      private boolean mousePressed = false;
      @Override
      public void mousePressed(MouseEvent mouseEvent){
        super.mousePressed(mouseEvent);
        mousePressed = true;
        new Thread(() ->{
          while(mousePressed){
            if(iterator.hasNext()){
              index.setText("current index: " + iterator.nextIndex());
              MoveDoubleLinkedList.DoubleLinkedListIterator dlli =
                      (MoveDoubleLinkedList.DoubleLinkedListIterator) iterator;
              String msg = dlli.isNextApplied() ? "applied" : "undo";
              //System.out.print(msg + " move: ");

              ChangeColorMove move = iterator.next();

              //System.out.println(move);

              repaint();
            }
            try{
              TimeUnit.MILLISECONDS.sleep(200);
            }catch(InterruptedException e){
              e.printStackTrace();
            }
          }
        }).start();
      }

      @Override
      public void mouseReleased(MouseEvent mouseEvent){
        super.mouseReleased(mouseEvent);
        mousePressed = false;
      }
    });

    prevButton.setSize(100, 50);
    nextButton.setSize(100, 50);
    buttonPanel.add(prevButton);
    buttonPanel.add(nextButton);
    add(buttonPanel, BorderLayout.SOUTH);

    setVisible(true);

  }
}
