#include "qtdiscrete.h"

N::Node:: Node     ( void )
        : node     ( 0    )
        , nodeType ( 0    )
        , flags    ( 0    )
        , name     ( ""   )
        , data     ( NULL )
{
}

N::Node:: Node     ( int Type )
        : node     ( 0        )
        , nodeType ( Type     )
        , flags    ( 0        )
        , name     ( ""       )
        , data     ( NULL     )
{
}

N::Node::~Node(void)
{
}

void N::Node::isolate(void)
{
  relations . clear ( ) ;
}

N::Node & N::Node::operator = (Node & n)
{
  return equalTo ( n ) ;
}

N::Node & N::Node::equalTo(Node & n)
{
  node      = n.node      ;
  nodeType  = n.nodeType  ;
  flags     = n.flags     ;
  name      = n.name      ;
  data      = n.data      ;
  paraments = n.paraments ;
  relations = n.relations ;
  return ME               ;
}

bool N::Node::Flag(SUID Mask)
{
  return ( ( flags & Mask ) == Mask ) ;
}

void N::Node::Marriage(Relation * relation)
{
  if (relations.contains(relation)) return ;
  relations << relation                    ;
}

void N::Node::Divorce(Relation * relation)
{
  if (!relations.contains(relation)) return ;
  int index = relations.indexOf(relation)   ;
  if (index<0) return                       ;
  relations . takeAt ( index )              ;
}

bool N::Node::isFirst(Relation * relation)
{
  return relation->isFirst(this) ;
}

bool N::Node::isEnd(Relation * relation)
{
  return relation->isEnd(this) ;
}

int N::Node::Connectors(void)
{
  return relations . count ( ) ;
}

N::Relation * N::Node::Connector (int index)
{
  if (relations.count()<=index) return NULL ;
  return relations[index]                   ;
}

bool N::clean(Nodes & nodes)
{
  int t = nodes.count()      ;
  nKickOut ( t <=0 , false ) ;
  for (int i=0;i<t;i++)      {
    delete nodes[i]          ;
  }                          ;
  nodes.clear()              ;
  return true                ;
}
