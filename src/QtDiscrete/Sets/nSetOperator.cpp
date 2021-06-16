#include "qtdiscrete.h"

N::SetOperator:: SetOperator ( QString a , QString b , Operators o )
               : N           ( ""                                  )
               , A           (         a                           )
               , B           (                     b               )
               , O           (                                   o )
{
}

N::SetOperator:: SetOperator ( QString a , QString b , QString o )
               : N           ( ""                                )
               , A           (         a                         )
               , B           (                     b             )
               , O           ( Nothing                           )
{
  if (o=="&" ) O = Intersection ; else
  if (o=="|" ) O = Union        ; else
  if (o=="-" ) O = Complement   ; else
  if (o=="\\") O = Complement   ; else
  if (o=="^" ) O = Symmetric    ;
}

N::SetOperator:: SetOperator ( void    )
               : N           ( ""      )
               , A           ( ""      )
               , B           ( ""      )
               , O           ( Nothing )
{
}

N::SetOperator::~SetOperator (void)
{
}

bool N::SetOperator::operator == (SetOperator & so)
{
  nKickOut ( so.O != O , false )              ;
  switch (O)                                  {
    case Nothing                              :
    return false                              ;
    case Intersection                         :
    case Union                                :
    case Symmetric                            :
      if (so.A!=A)                            {
        nKickOut ( so.A != B , false )        ;
        nKickOut ( so.B != A , false )        ;
      } else                                  {
        nKickOut ( so.B != B , false )        ;
      }                                       ;
    break                                     ;
    case Complement                           :
      nKickOut ( so.A != A , false )          ;
      nKickOut ( so.B != B , false )          ;
    break                                     ;
    default                                   :
    return false                              ;
  }                                           ;
  return true                                 ;
}

QString N::SetOperator::Syntax(void)
{
  switch (O)                                     {
    case Nothing                                 :
    return ""                                    ;
    case Intersection                            :
    return QString("( %1 & %2 )" ).arg(A).arg(B) ;
    case Union                                   :
    return QString("( %1 | %2 )" ).arg(A).arg(B) ;
    case Complement                              :
    return QString("( %1 \\ %2 )").arg(A).arg(B) ;
    case Symmetric                               :
    return QString("( %1 ^ %2 )" ).arg(A).arg(B) ;
  }                                              ;
  return ""                                      ;
}

N::VarArgs N::SetOperator::Arguments(void)
{
  VarArgs NVA                  ;
  QString  M = QString("Sets") ;
  NVA << QVariant(      M    ) ;
  NVA << QVariant((int) O    ) ;
  NVA << QVariant(      A    ) ;
  NVA << QVariant(      B    ) ;
  return NVA                   ;
}

bool N::SetOperator::Assign(QString syntax)
{
  nKickOut ( !syntax.contains('(') , false ) ;
  nKickOut ( !syntax.contains(')') , false ) ;
  QString     s = syntax                     ;
  QStringList v                              ;
  QStringList r                              ;
  s = s.replace  ( "("  , ""     )           ;
  s = s.replace  ( ")"  , ""     )           ;
  s = s.replace  ( "&"  , " & "  )           ;
  s = s.replace  ( "|"  , " | "  )           ;
  s = s.replace  ( "-"  , " - "  )           ;
  s = s.replace  ( "\\" , " \\ " )           ;
  s = s.replace  ( "^"  , " ^ "  )           ;
  s . simplified (               )           ;
  while (s.contains("  "))                   {
    s = s.replace("  "," ")                  ;
  }                                          ;
  r = s.split    ( ' '         )             ;
  foreach (s,r) if (s.length()>0) v << s     ;
  nKickOut ( v.count() != 3        , false ) ;
  O = Nothing                                ;
  if (v[1]=="&" ) O = Intersection      ; else
  if (v[1]=="|" ) O = Union             ; else
  if (v[1]=="-" ) O = Complement        ; else
  if (v[1]=="\\") O = Complement        ; else
  if (v[1]=="^" ) O = Symmetric         ; else
    return false                             ;
  A = v[0]                                   ;
  B = v[2]                                   ;
  return true                                ;
}

bool N::SetOperator::Execute(UUIDs & a,UUIDs & b,UUIDs & r)
{
  RMAPs X                           ;
  XMAPs S                           ;
  UUIDs T                           ;
  SUID  U                           ;
  ///////////////////////////////////
  r . clear ( )                     ;
  ///////////////////////////////////
  switch (O)                        {
    case Intersection               :
      if (a.count()<=0) return true ;
      if (b.count()<=0) return true ;
      UuidInt    ( a , 1 , X )      ;
      UuidAddInt ( b , 1 , X )      ;
      T = X.keys()                  ;
      foreach (U,T)                 {
        if (X[U]==2) r << U         ;
      }                             ;
    return true                     ;
    case Union                      :
      if (a.count()<=0)             {
        r = b                       ;
        return true                 ;
      }                             ;
      if (b.count()<=0)             {
        r = a                       ;
        return true                 ;
      }                             ;
      UuidBoolean(a,true,S)         ;
      UuidBoolean(b,true,S)         ;
      r = S.keys()                  ;
    return true                     ;
    case Complement                 :
      if (a.count()<=0) return true ;
      if (b.count()<=0)             {
        r = a                       ;
        return true                 ;
      }                             ;
      UuidBoolean(a,true ,S)        ;
      UuidBoolean(b,false,S)        ;
      T = S.keys()                  ;
      foreach (U,T)                 {
        if (X[U]) r << U            ;
      }                             ;
    return true                     ;
    case Symmetric                  :
      if (a.count()<=0) return true ;
      if (b.count()<=0) return true ;
      UuidInt    ( a , 1 , X )      ;
      UuidAddInt ( b , 1 , X )      ;
      T = X.keys()                  ;
      foreach (U,T)                 {
        if (X[U]==1) r << U         ;
      }                             ;
    return true                     ;
  }                                 ;
  return false                      ;
}
