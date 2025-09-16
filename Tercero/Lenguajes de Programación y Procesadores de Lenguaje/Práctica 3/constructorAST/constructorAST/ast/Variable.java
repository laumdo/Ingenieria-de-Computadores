package ast;

public class Variable extends E {
  private String v;
  public Variable(String v) {
   this.v = v;
  }
  public String variable() {return v;}
  public KindE kind() {return KindE.VARIABLE;}
  public String toString() {return v;}
}
