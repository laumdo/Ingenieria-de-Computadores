package alex;

import eval.ClaseLexica;

public class ALexOperations {
  private AnalizadorLexicoEval alex;
  public ALexOperations(AnalizadorLexicoEval alex) {
   this.alex = alex;   
  }
  public UnidadLexica unidadWh() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.WH);
  }
  public UnidadLexica unidadFor() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.FOR);
  }
  public UnidadLexica unidadSi() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.SI);
  }
  public UnidadLexica unidadSino() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.SINO);
  }
  public UnidadLexica unidadHaz() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.HAZ);
  }
  public UnidadLexica unidadMientras() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.MIENTRAS);
  }
  public UnidadLexica unidadTipoEnt() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.TENT
                                         );
  }
  public UnidadLexica unidadTipoDec() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.TDEC
                                         );
  }
  public UnidadLexica unidadTipoBooleano(){
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.TBOOL
                                         );
  }

  public UnidadLexica unidadId() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.IDEN,
                                         alex.lexema());
  }
  public UnidadLexica unidadEnt() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.ENT,alex.lexema());
  }
  public UnidadLexica unidadReal() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.REAL,alex.lexema());
  }

  public UnidadLexica unidadSuma() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.MAS);
  }
  public UnidadLexica unidadResta() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.MENOS);
  }
  public UnidadLexica unidadMul() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.POR);
  }
  public UnidadLexica unidadDiv() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.DIV);
  }
  public UnidadLexica unidadPAp() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.PAP);
  }
  public UnidadLexica unidadIncremento() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.INCREMENTO);
  }
  public UnidadLexica unidadDecremento() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.DECREMENTO);
  }
  public UnidadLexica unidadPCierre() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.PCIERRE);
  }
  public UnidadLexica unidadIgual() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.IGUAL);
  }
  public UnidadLexica unidadComa() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.COMA);
  }
  public UnidadLexica unidadCorcheteApertura() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.CORCHETEAPERTURA);
  }
  public UnidadLexica unidadCorcheteCierre() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.CORCHETECIERRE);
  }
  public UnidadLexica unidadPunto() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.PUNTO);
  }
  public UnidadLexica unidadPuntoComa() {
   return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.PUNTOCOMA);
  }
  public UnidadLexica unidadMayor() {
   return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.MAYOR);
  } 
  public UnidadLexica unidadMenor() {
   return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.MENOR);
} 
public UnidadLexica unidadIgualIgual() {
   return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.IGIGUAL);
}
  public UnidadLexica unidadEof() {
     return new UnidadLexica(alex.fila(),alex.columna(),ClaseLexica.EOF);
  }
  public void error() {
    System.err.println("***"+alex.fila()+", "+alex.columna()+" Caracter inesperado: "+alex.lexema());
  }
}
