package ast;

public abstract class IASIGNA extends I {
   private String id;
   private E value;

   public IASIGNA(String ident, E val) {
     this.id=ident;
     this.value=val;
   }

   public String tostring(){
    return "asigna(" + id +"," +value.tostring()+")";
   }
}
