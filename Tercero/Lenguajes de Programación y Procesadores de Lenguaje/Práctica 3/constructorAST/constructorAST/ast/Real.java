package ast;

public class Real extends E {
  private String v;
  public Real(String v) {
   this.v = v;
  }
  public String real() {return v;}
  public KindE kind() {return KindE.REAL;}
  public String toString() {return v;}  
}
