#include "qtdiscrete.h"

N::Tree:: Tree       (void)
        : Graph      (    )
        , root       (NULL)
        , RelationId (0   )
{
}

N::Tree::~Tree (void)
{
}

void N::Tree::setRoot(Node * node)
{
  if (addNode(node)) {
    root = node      ;
  } else             {
    root = node      ;
  }                  ;
}

int N::Tree::parenting(Node * father,Node * child)
{
  Relation * r                            ;
  r = new Relation ( RelationId , 0     ) ;
  Mount            ( r , father , child ) ;
  RelationId ++                           ;
  return RelationId                       ;
}
