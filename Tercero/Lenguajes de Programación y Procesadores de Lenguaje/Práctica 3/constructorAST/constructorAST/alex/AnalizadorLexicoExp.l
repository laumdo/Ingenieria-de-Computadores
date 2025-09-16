package alex;

import errors.GestionErroresExp;

%%
%cup
%column
%line
%class AnalizadorLexicoExp
%unicode
%public

%{
  private ALexOperations ops;
  private GestionErroresExp errores;
  public String lexema() {return yytext();}
  public int fila() {return yyline+1;}
  public int columna() {return yycolumn+1;}
  public void fijaGestionErrores(GestionErroresExp errores) {
   this.errores = errores;
  }
%}

%eofval{
  return ops.unidadEof();
%eofval}

%init{
  ops = new ALexOperations(this);
%init}

letra  = ([A-Z]|[a-z])
digitoPositivo = [1-9]
digito = ({digitoPositivo}|0)
parteEntera = {digitoPositivo}{digito}*
parteDecimal = {digito}* {digitoPositivo}
separador = [ \t\n]
comentario = #[^\n]*
identificador = {letra}({letra}|{digito})*
numeroEntero = [\+\-]?{digito}+
numeroReal = [\+\-]?{parteEntera}\.{parteDecimal}
operadorSuma = \+
operadorResta = \-
operadorMultiplicacion = \*
operadorDivision = \/
parentesisApertura = \(
parentesisCierre = \)
igual = \=
coma  = ,
corcheteApertura = \{
corcheteCierre = \}
punto = \.
puntoComa = ;
mayor = >
menor = <
incremento = \+\+
decremento = \-\-
igualIgual = \=\=
palabraClaveWh = "wh"
palabraClaveFor = "for"
palabraClaveSi = "si"
palabraClaveSino = "sino"
palabraClaveHaz = "haz"
palabraClaveMientras = "mientras"
palabraClaveDev = "dev"
tipoDatoEnt = "ent"
tipoDatoDec = "dec"
tipoDatoBooleano = "booleano"

%%

{separador}               {}
{comentario}              {}
{palabraClaveWh}          { return ops.unidadWh(); }
{palabraClaveFor}         { return ops.unidadFor(); }
{palabraClaveSi}          { return ops.unidadSi(); }
{palabraClaveSino}        { return ops.unidadSino(); }
{palabraClaveHaz}         { return ops.unidadHaz(); }
{palabraClaveMientras}    { return ops.unidadMientras(); }
{tipoDatoEnt}             { return ops.unidadTipoEnt(); }
{tipoDatoDec}             { return ops.unidadTipoDec(); }
{tipoDatoBooleano}        { return ops.unidadTipoBooleano(); }
{identificador}           { return ops.unidadId(); }
{numeroEntero}            { return ops.unidadEnt(); }
{numeroReal}              { return ops.unidadReal(); }
{operadorSuma}            { return ops.unidadSuma(); }
{operadorResta}           { return ops.unidadResta(); }
{operadorMultiplicacion}  { return ops.unidadMul(); }
{operadorDivision}        { return ops.unidadDiv(); }
{parentesisApertura}      { return ops.unidadPAp(); }
{parentesisCierre}        { return ops.unidadPCierre(); }
{igual}                   { return ops.unidadIgual(); }
{coma}                    { return ops.unidadComa(); }
{corcheteApertura}        { return ops.unidadCorcheteApertura(); }
{corcheteCierre}          { return ops.unidadCorcheteCierre(); }
{punto}                   { return ops.unidadPunto(); }
{puntoComa}               { return ops.unidadPuntoComa(); }
{mayor}                   { return ops.unidadMayor(); }
{menor}                   { return ops.unidadMenor(); }
{incremento}              { return ops.unidadIncremento(); }
{decremento}              { return ops.unidadDecremento(); }
{igualIgual}              { return ops.unidadIgualIgual();}
[^]                       { ops.error(); }
