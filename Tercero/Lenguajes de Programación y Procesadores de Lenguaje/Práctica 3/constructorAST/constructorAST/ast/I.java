package ast;

public abstract class I implements ASTNode {

    public abstract KindI kind();
    public NodeKind nodeKind() {return NodeKind.INSTR;}
    public String toString() {return "";}

}
