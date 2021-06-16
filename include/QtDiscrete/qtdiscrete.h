/****************************************************************************
 *                                                                          *
 * Copyright (C) 2015 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 * Qt Version   : 5.4.1                                                     *
 * CIOS Version : 1.6.0                                                     *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_DISCRETE_H
#define QT_DISCRETE_H

#include <QtCore>
#include <Essentials>
#include <QtGMP>
#include <QtGSL>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTDISCRETE_LIB)
#      define Q_DISCRETE_EXPORT Q_DECL_EXPORT
#    else
#      define Q_DISCRETE_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_DISCRETE_EXPORT
#endif

namespace N
{

class Q_DISCRETE_EXPORT SetOperator ;
class Q_DISCRETE_EXPORT SetsAlgebra ;
class Q_DISCRETE_EXPORT Node        ;
class Q_DISCRETE_EXPORT Relation    ;
class Q_DISCRETE_EXPORT Binary      ;
class Q_DISCRETE_EXPORT Graph       ;
class Q_DISCRETE_EXPORT Tree        ;
class Q_DISCRETE_EXPORT OcTree      ;

typedef QList < Node     * > Nodes     ;
typedef QList < Relation * > Relations ;

/*****************************************************************************
 *                                                                           *
 *                                 Sets theory                               *
 *                                                                           *
 *****************************************************************************/

class Q_DISCRETE_EXPORT SetOperator
{
  public:

    typedef enum       {
      Nothing      = 0 , // Actually, this is invalid operator
      Intersection = 1 , // A & B , And       , with operator &
      Union        = 2 , // A | B , Or        , with operator |
      Complement   = 3 , // B - A , Substract , with operator / or -
                         // as a formal notation B - A should be written as
                         // B \ A , read ISO 31-11 standard for details
      Symmetric    = 4   // Symmetric difference , with operator ^
                         // ( A \ B ) | ( B \ A )
    } Operators        ;

    QString   N ;
    QString   A ;
    QString   B ;
    Operators O ;

    explicit SetOperator  (QString a,QString b,Operators o) ;
    explicit SetOperator  (QString a,QString b,QString   o) ;
    explicit SetOperator  (void) ;
    virtual ~SetOperator  (void) ;

    bool     operator  == (SetOperator & so) ;
    QString  Syntax       (void) ;
    VarArgs  Arguments    (void) ;
    bool     Assign       (QString syntax) ;
    bool     Execute      (UUIDs & a,UUIDs & b,UUIDs & r) ;

  protected:

  private:

};

class Q_DISCRETE_EXPORT SetsAlgebra
{
  public:

    SUID                        Uuid          ;
    QString                     Identifier    ;
    QString                     Expression    ;
    QString                     Result        ;
    QStringList                 Inputs        ;
    QStringList                 Elements      ;
    QStringList                 Sequences     ;
    QList<SetOperator        *> Operations    ;
    QMap<QString,SetOperator *> OpMaps        ;
    QMap<QString,UUIDs        > Uuids         ;
    int                         Intermediates ;

    explicit SetsAlgebra  (QString expression) ;
    explicit SetsAlgebra  (void) ;
    virtual ~SetsAlgebra  (void) ;

    void     clear        (void) ;
    void     EmptyUuids   (void) ;
    bool     AssignUuids  (QString name,UUIDs & uuids) ;
    int      Total        (void) ;
    bool     Decode       (QString expression) ;
    bool     Execute      (UUIDs & Results) ;
    QString  Expanding    (int level,int startId,QString op) ;
    QString  Construct    (void) ;
    VarArgs  Arguments    (QString name) ;
    VarArgs  Arguments    (int index) ;

  protected:

    int      Count        (QString expr,QChar c) ;
    bool     addCompose   (QString expression,QString & name) ;
    bool     addOperator  (QString token,QString & name) ;
    bool     addElement   (QString name) ;
    bool     addInput     (QString name) ;
    bool     isPure       (QString token) ;
    QString  Companion    (QString token) ;
    QString  Construct    (QString element) ;

  private:

};

/*****************************************************************************
 *                                                                           *
 *                               Discrete Math                               *
 *                                                                           *
 *****************************************************************************/

class Q_DISCRETE_EXPORT Node
{
  public:

    SUID      node      ;
    int       nodeType  ;
    SUID      flags     ;
    QString   name      ;
    void    * data      ;
    VarArgs   paraments ;
    Relations relations ;

    explicit   Node       (void) ;
    explicit   Node       (int Type) ;
    virtual ~  Node       (void) ;

    void       isolate    (void) ;

    Node &     operator = (Node & node) ;
    Node &     equalTo    (Node & node) ;
    bool       Flag       (SUID Mask) ;

    void       Marriage   (Relation * relation) ;
    void       Divorce    (Relation * relation) ;

    bool       isFirst    (Relation * relation) ;
    bool       isEnd      (Relation * relation) ;

    int        Connectors (void) ;
    Relation * Connector  (int index) ;

  protected:

  private:

};

Q_DISCRETE_EXPORT bool clean (Nodes & nodes) ;

class Q_DISCRETE_EXPORT Relation
{
  public:

    SUID    relation  ;
    int     linkType  ;
    SUID    flags     ;
    QString name      ;
    void  * data      ;
    VarArgs arguments ;
    Nodes   nodes     ;

    explicit Relation     (SUID relate,int linkType) ;
    explicit Relation     (void) ;
    virtual ~Relation     (void) ;

    Relation & operator = (Relation & relation) ;
    Relation & equalTo    (Relation & relation) ;

    void connect          (Node & first,Node & second) ;
    void disconnect       (void) ;

    // More than binary relation
    int  join             (Node & node) ;
    int  operator      += (Node & node) ;

    int  Indexing         (Node * node) ;
    bool isFirst          (Node * node) ;
    bool isEnd            (Node * node) ;
    bool isDating         (void) ;

    bool is               (N::Groups::Relations relate) ;

    int  connexion        (void) const ;

  protected:

  private:

};

class Q_DISCRETE_EXPORT Binary
{
  public:

    Nodes    nodes    ;
    Relation relation ;

    explicit Binary (void) ;
    virtual ~Binary (void) ;

    void Purify     (void) ;
    void Relate     (SUID first,SUID second,int relation) ;

    // A=B => O(A) R O(B) in functional expression , where R = Neutrino::Groups::Equivalent
    // A@B => O(A) R O(B) in functional expression , where R = Neutrino::Groups::Contains
    // A>B => O(A) R O(B) in functional expression , where R = Neutrino::Groups::Greater
    // A<B => O(A) R O(B) in functional expression , where R = Neutrino::Groups::Less
    bool Pair       (QString valuePair) ;

    bool SqlWhere   (QString     & Where          ,
                     QStringList & Keys           ,
                     KMAPs       & Maps           ,
                     QString       Options = "" ) ;

  protected:

  private:

};

class Q_DISCRETE_EXPORT Graph
{
  public:

    Nodes     nodes    ;
    Relations linkers  ;
    bool      destruct ;

    explicit Graph          (void) ;
    virtual ~Graph          (void) ;

    bool    contains        (Node * node) ;

    int     Members         (void) ;
    Node  * Member          (int index) ;

    void    Purify          (void) ;

    bool    addRelation     (Relation * relation) ;
    bool    removeRelation  (Relation * relation) ;

    bool    addNode         (Node * node) ;
    bool    removeNode      (Node * node) ;

    virtual void detachNode (Node * node) ;

    void    Mount           (Relation * relation,Node * first,Node * second) ;

  protected:

  private:

};

class Q_DISCRETE_EXPORT Tree : public Graph
{
  public:

    Node * root ;

    explicit Tree         (void) ;
    virtual ~Tree         (void) ;

    void setRoot          (Node * node) ;

    virtual int parenting (Node * father,Node * child) ;

  protected:

    int RelationId ;

  private:

};

/*****************************************************************************
 *                                                                           *
 *                                Octant Tree                                *
 *                                                                           *
 *****************************************************************************/

template<typename T1,typename T2>
class OcTraits
{
  public:

    typedef T1 Result;

} ;

template<>
class OcTraits<int,double>
{
  public:

    typedef double Result;

} ;

template<>
class OcTraits<double,int>
{
  public:

    typedef double Result;

} ;

template<>
class OcTraits<int,float>
{
  public:

    typedef float Result;

} ;

template<>
class OcTraits<float,int>
{
  public:

    typedef float Result;

} ;

template<typename T>
class OcShared : public QSharedData
{
  public:

    OcShared (T * data = 0) : QSharedData (      )
                            , data_       ( data )
    {
    }

    OcShared (const OcShared<T> & sharedData) : QSharedData (                  )
                                              , data_       ( sharedData.data_ )
    {
    }

    virtual ~OcShared (void)
    {
      if ( data_ != NULL ) {
        delete [] data_    ;
        data_   = NULL     ;
      }                    ;
    }

    void swap (OcShared<T> & sharedData)
    {
      T * t            = data_            ;
      data_            = sharedData.data_ ;
      sharedData.data_ = t                ;
    }

    OcShared<T> & operator= (OcShared<T> sharedData )
    {
      swap ( sharedData ) ;
      return (*this)      ;
    }

    T & operator[] ( int i ) const
    {
      OcAssert ( data_ ) ;
      return  data_[i] ;
    }

  protected:

    T * data_ ;

  private:

};

template<typename T,int N>
class OcVector
{
  public:

    OcVector (void)
    {
    }

    OcVector (const T & value)
    {
      for ( int i = 0; i < N; ++i ) {
        data_ [ i ] = value         ;
      }                             ;
    }

    virtual ~OcVector (void)
    {
    }

    template<typename T2>
    OcVector(const OcVector<T2,N> & v)
    {
      (*this) = v ;
    }

    template<typename T2>
    OcVector<T,N> & operator = (const OcVector<T2,N> & v)
    {
      for ( int i = 0; i < N; ++i ) {
        data_[i] = v(i)             ;
      }                             ;
      return (*this)                ;
    }

    const T & at(int i) const
    {
      return data_[i] ;
    }

    const T & operator() (int i) const
    {
      return at(i) ;
    }

    T & operator() (int i)
    {
      return data_[i] ;
    }

    const T * data(void) const
    {
      return data_ ;
    }

    T * data(void)
    {
      return data_ ;
    }

    void operator += (const OcVector<T,N> & v)
    {
      for ( int i = 0; i < N; ++i ) {
        data_[i] += v.data_[i]      ;
      }                             ;
    }

    void operator -= (const OcVector<T,N> & v)
    {
      for ( int i = 0; i < N; ++i ) {
        data_[i] -= v.data_[i]      ;
      }                             ;
    }

    void operator *= (const OcVector<T,N> & v)
    {
      for ( int i = 0; i < N; ++i ) {
        data_[i] *= v.data_[i]      ;
      }                             ;
    }

    void operator *= (const T & x)
    {
      for ( int i = 0; i < N; ++i ) {
        data_[i] *= x               ;
      }                             ;
    }

    void operator /= (const T& x)
    {
      for ( int i = 0; i < N; ++i ) {
        data_[i] /= x               ;
      }                             ;
    }

  private:

    T data_ [ N ] ;

};

template<typename T1,typename T2,int N>
OcVector<typename OcTraits<T1,T2>::Result,N> operator + (
                  const OcVector<T1,N> & v1             ,
                  const OcVector<T2,N> & v2             )
{
  OcVector<typename OcTraits<T1,T2>::Result,N> v3(v1) ;
  v3 += v2                                            ;
  return v3                                           ;
}

template< typename T1, typename T2, int N >
OcVector< typename OcTraits<T1,T2>::Result,N> operator - (
                   const OcVector<T1,N> & v1             ,
                   const OcVector<T2,N> & v2             )
{
  OcVector<typename OcTraits<T1,T2>::Result,N> v3(v1) ;
  v3 -= v2                                            ;
  return v3                                           ;
}

template<typename T1,typename T2,int N>
OcVector<typename OcTraits<T1,T2>::Result,N> operator * (
                  const OcVector<T1,N> & v1             ,
                  const OcVector<T2,N> & v2             )
{
  OcVector<typename OcTraits<T1,T2>::Result,N> v3(v1) ;
  v3 *= v2                                            ;
  return v3                                           ;
}

template<typename T1,typename T2,int N>
OcVector<typename OcTraits<T1,T2>::Result,N> operator * (
                  const OcVector<T1,N> & v1             ,
                  const T2             & x              )
{
  OcVector<typename OcTraits<T1,T2>::Result,N> v2(v1) ;
  v2 *= x                                             ;
  return v2                                           ;
}

template<typename T,int N>
OcVector<T,N> operator * (const T & x,OcVector<T,N> v1)
{
  for ( int i = 0; i < N; ++i ) {
    v1(i) = x * v1(i)           ;
  }                             ;
  return v1                     ;
}

template<typename T1,typename T2,int N>
OcVector<typename OcTraits<T1,T2>::Result,N> operator * (
                  const T2             & x              ,
                  const OcVector<T1,N> & v1             )
{
  return x * OcVector<typename OcTraits<T1,T2>::Result,N>(v1) ;
}

template< typename T1, typename T2, int N >
OcVector< typename OcTraits<T1,T2>::Result, N > operator / (
                   const OcVector<T1,N> & v1               ,
                   const OcVector<T2,N> & v2               )
{
  OcVector<typename OcTraits<T1,T2>::Result,N> v3(v1) ;
  v3 /= v2                                            ;
  return v3                                           ;
}

template<typename T1,typename T2,int N>
OcVector<typename OcTraits<T1,T2>::Result,N> operator / (
                  const OcVector<T1,N> & v1             ,
                  const T2             & x              )
{
  OcVector<typename OcTraits<T1,T2>::Result,N> v2(v1) ;
  v2 /= x                                             ;
  return v2                                           ;
}

template<typename T,int N>
T sum(const OcVector<T,N> & v)
{
  T s(0)                        ;
  for ( int i = 0; i < N; ++i ) {
    s += v(i)                   ;
  }                             ;
  return s                      ;
}

template<typename T,int N>
T prod(const OcVector<T,N> & v)
{
  T p(1)                        ;
  for ( int i = 0; i < N; ++i ) {
    p *= v(i)                   ;
  }                             ;
  return p                      ;
}

template<typename T,int N>
OcVector<T,N> cumprod(const OcVector<T,N> & v)
{
  OcVector<T,N> p               ;
  T             prev(1)         ;
  for ( int i = 0; i < N; ++i ) {
    p(i) = prev = prev * v(i)   ;
  }                             ;
  return p                      ;
}

template<typename T,int N>
T norm(const OcVector<T,N> & v)
{
  return sqrt( sum ( v * v ) ) ;
}

template<typename T>
class OcPoint : public OcVector<T,3>
{
  public:

    OcPoint(void)
    {
    }

    OcPoint(T x,T y,T z)
    {
      (*this)(0) = x ;
      (*this)(1) = y ;
      (*this)(2) = z ;
    }

    OcPoint(const OcVector<T,3> & v) : OcVector<T,3>(v)
    {
    }

    const T & x (void) const { return this->at(0) ; }
    const T & y (void) const { return this->at(1) ; }
    const T & z (void) const { return this->at(2) ; }
          T & x (void)       { return (*this) (0) ; }
          T & y (void)       { return (*this) (1) ; }
          T & z (void)       { return (*this) (2) ; }

};

template<typename T,int N>
class OcArray
{
  public:

    OcArray (void) : data_  (NULL)
                   , sizes_ (0   )
    {
    }

    OcArray (const OcVector<int,N> & sizes ) : data_    (new T[prod(sizes)]     )
                                             , offset_  (0                      )
                                             , strides_ (cumprod(sizes)/sizes(0))
                                             , sizes_   (sizes                  )
    {
    }

    const OcVector<int,N> & sizes(void) const
    {
      return sizes_ ;
    }

    const T & at(const OcVector<int,N> & indices) const
    {
      return data_ [ dataIndex ( indices ) ] ;
    }

    const T & operator() (const OcVector<int,N> & indices) const
    {
      return at ( indices ) ;
    }

    T & operator() (const OcVector<int,N> & indices)
    {
      return data_ [ dataIndex ( indices ) ] ;
    }

    OcArray<T,N> subarray(const OcVector<int,N> & begin ,
                          const OcVector<int,N> & end   )
    {
      OcArray<T,N>   sub                 ;
      sub.data_    = data_               ;
      sub.offset_  = dataIndex ( begin ) ;
      sub.strides_ = strides_            ;
      sub.sizes_   = end - begin         ;
      return         sub                 ;
    }

  protected:

    OcShared<T>     data_    ;
    int             offset_  ;
    OcVector<int,N> strides_ ;
    OcVector<int,N> sizes_   ;

    int dataIndex (const OcVector<int,N> & indices) const
    {
      for ( int i = 0; i < N; ++i )                         {
        OcAssert( indices(i) >= 0 && indices(i) < sizes_(i) ) ;
      }                                                     ;
      return offset_ + sum ( indices * strides_ )           ;
    }

  private:

};

template<typename T>
class OcMatrix : public OcArray<T,2>
{
  public:

    OcMatrix(void) : OcArray<T,2>()
    {
    }

    OcMatrix(int m,int n) : OcArray<T,2>()
    {
      OcVector<int,2> sizes                            ;
      sizes(1) = m                                     ;
      sizes(0) = n                                     ;
      OcArray<T,2>::operator = ( OcArray<T,2>(sizes) ) ;
    }

    OcMatrix(const OcArray<T,2> & array) : OcArray<T,2>(array)
    {
    }

    int M(void) const
    {
      return this->sizes()(1) ;
    }

    int N(void) const
    {
      return this->sizes()(0) ;
    }

    const T & at(int i,int j) const
    {
      OcVector<int,2> indices          ;
      indices(1) = i                   ;
      indices(0) = j                   ;
      return OcArray<T,2>::at(indices) ;
    }

    const T & operator() (int i,int j) const
    {
      return at ( i , j ) ;
    }

    T & operator() ( int i, int j )
    {
      OcVector<int,2> indices                  ;
      indices(1) = i                           ;
      indices(0) = j                           ;
      return OcArray<T,2>::operator()(indices) ;
    }

    OcMatrix<T> subarray(int iBegin,int jBegin,int iEnd,int jEnd)
    {
      OcVector<int,2> begin                          ;
      OcVector<int,2> end                            ;
      begin ( 1 )   = iBegin                         ;
      begin ( 0 )   = jBegin                         ;
      end   ( 1 )   = iEnd                           ;
      end   ( 0 )   = jEnd                           ;
      return OcArray<T,2> :: subarray ( begin, end ) ;
    }

};

template<typename T,int AS = 1>
class OctantTree
{
  protected:

    enum NodeType    {
      BranchNode     ,
      AggregateNode  ,
      LeafNode     } ;

    template<typename X,int as>
    class OcNode
    {
      public:

        OcNode(NodeType type) : type_ ( type )
        {
        }

        virtual ~OcNode(void)
        {
        }

        NodeType type(void) const
        {
          return type_ ;
        }

      protected:

        NodeType type_ ;

      private:

    };

    template<typename X,int as>
    class OcBranch : public OcNode<X,as>
    {
      public:

        OcBranch(void) : OcNode<X,as>(BranchNode)
        {
          children [0][0][0] = NULL ;
          children [0][0][1] = NULL ;
          children [0][1][0] = NULL ;
          children [0][1][1] = NULL ;
          children [1][0][0] = NULL ;
          children [1][0][1] = NULL ;
          children [1][1][0] = NULL ;
          children [1][1][1] = NULL ;
        }

        OcBranch(const OcBranch<X,as> & b) : OcNode<X,as>(BranchNode)
        {
          for ( int i = 0; i < 8; ++i )     {
            if ( b.child(i) )               {
              switch ( b.child(i)->type() ) {
                case BranchNode             :
                  child(i) = new OcBranch   <X,as>(*reinterpret_cast<const OcBranch   <X,as>*>(b.child(i))) ;
                break                       ;
                case LeafNode               :
                  child(i) = new OcLeaf     <X,as>(*reinterpret_cast<const OcLeaf     <X,as>*>(b.child(i))) ;
                break                       ;
                case AggregateNode          :
                  child(i) = new OcAggregate<X,as>(*reinterpret_cast<const OcAggregate<X,as>*>(b.child(i))) ;
                break                       ;
              }                             ;
            } else child(i) = 0             ;
          }                                 ;
        }

        virtual ~OcBranch(void)
        {
          for ( int i = 0; i < 2; ++i )                   {
            for ( int j = 0; j < 2; ++j )                 {
              for ( int k = 0; k < 2; ++k )               {
                OcAssert   (  children[i][j][k] != this ) ;
                deleteNode ( &children[i][j][k]         ) ;
              }                                           ;
            }                                             ;
          }                                               ;
        }

        const OcNode<X,as> *   child (int x,int y,int z) const
        {
          OcAssert ( x == 0 || x == 1 ) ;
          OcAssert ( y == 0 || y == 1 ) ;
          OcAssert ( z == 0 || z == 1 ) ;
          return children [z][y][x]     ;
        }

              OcNode<X,as> * & child (int x,int y,int z)
        {
          OcAssert ( x == 0 || x == 1 ) ;
          OcAssert ( y == 0 || y == 1 ) ;
          OcAssert ( z == 0 || z == 1 ) ;
          return children [z][y][x]     ;
        }

        const OcNode<X,as> *   child (int index) const
        {
          OcAssert( index >= 0 && index < 8    ) ;
          return *( &children[0][0][0] + index ) ;
        }

              OcNode<X,as> * & child (int index)
        {
          OcAssert( index >= 0 && index < 8    ) ;
          return *( &children[0][0][0] + index ) ;
        }

      protected:

        OcNode<X,as> * children[2][2][2] ;

      private:

    };

    template<typename X,int as>
    class OcAggregate : public OcNode<X,as>
    {
      public:

        OcAggregate (const X & v) : OcNode<X,as>(AggregateNode)
        {
          for ( int i = 0; i < as; ++i )     {
            for ( int j = 0; j < as; ++j )   {
              for ( int k = 0; k < as; ++k ) {
                value_[i][j][k] = v          ;
              }                              ;
            }                                ;
          }                                  ;
        }

        virtual ~OcAggregate(void)
        {
        }

        const X & value(int x,int y,int z) const
        {
          OcAssert ( x >= 0 && x < AS ) ;
          OcAssert ( y >= 0 && y < AS ) ;
          OcAssert ( z >= 0 && z < AS ) ;
          return value_[z][y][x]        ;
        }

        X & value(int x,int y,int z)
        {
          OcAssert ( x >= 0 && x < AS ) ;
          OcAssert ( y >= 0 && y < AS ) ;
          OcAssert ( z >= 0 && z < AS ) ;
          return value_[z][y][x]        ;
        }

        void setValue(int x,int y,int z,const X & v)
        {
          OcAssert ( x >= 0 && x < as ) ;
          OcAssert ( y >= 0 && y < as ) ;
          OcAssert ( z >= 0 && z < as ) ;
          value_ [z][y][x] = v          ;
        }

        const X & value(int i) const
        {
          OcAssert( i >= 0 && i < as * as * as ) ;
          return *( &value_[0][0][0] + i       ) ;
        }

        X & value(int i)
        {
          OcAssert( i >= 0 && i < as * as * as ) ;
          return *( &value_[0][0][0] + i       ) ;
        }

        void setValue(int i,const T & v)
        {
          OcAssert( i >= 0 && i < as * as * as ) ;
          *( &value_[0][0][0] + i ) = v          ;
        }

      protected:

        X value_ [as][as][as] ;

      private:

    };

    template<typename X,int as>
    class OcLeaf : public OcNode<X,as>
    {
      public:

        OcLeaf (const X & v) : OcNode<X,as> ( LeafNode )
                             , value_       ( v        )
        {
        }

        virtual ~OcLeaf(void)
        {
        }

        const X & value(void) const
        {
          return value_ ;
        }

        X & value(void)
        {
          return value_ ;
        }

        void setValue(const X & v)
        {
          value_ = v ;
        }

      protected:

        X value_ ;

      private:

    };

    static const int aggregateSize_ = AS ;
    OcNode<T,AS>   * root_               ;
    T                emptyValue_         ;
    int              size_               ;

  public:

    OctantTree (int size,const T & emptyValue = T(0)) : root_       (0         )
                                                      , emptyValue_ (emptyValue)
                                                      , size_       (size      )
    {
      // Make sure size is power of two.
      OcAssert( ((size - 1) & size) == 0 ) ;
      OcAssert( ((AS   - 1) & AS  ) == 0 ) ;
    }

    OctantTree (const OctantTree<T,AS> & o) : emptyValue_ ( o.emptyValue_ )
                                            , size_       ( o.size_       )
    {
      if ( !o.root_ ) root_ = NULL ; else {
        switch ( o.root_->type() )        {
          case BranchNode                 :
            root_ = (OcNode<T,AS> *)new OcBranch   <T,AS>( *reinterpret_cast<OcBranch   <T,AS>*>(o.root_) ) ;
          break                           ;
          case LeafNode                   :
            root_ = (OcNode<T,AS> *)new OcLeaf     <T,AS>( *reinterpret_cast<OcLeaf     <T,AS>*>(o.root_) ) ;
          break                           ;
          case AggregateNode              :
            root_ = (OcNode<T,AS> *)new OcAggregate<T,AS>( *reinterpret_cast<OcAggregate<T,AS>*>(o.root_) ) ;
          break                           ;
        }                                 ;
      }                                   ;
    }

    virtual ~OctantTree(void)
    {
      deleteNode ( &root_ ) ;
    }

    // Accessors
    int size(void) const
    {
      return size_ ;
    }

    const T & emptyValue(void) const
    {
      return emptyValue_ ;
    }

    static unsigned long branchBytes(void)
    {
      return sizeof(OcBranch<T,AS>) ;
    }

    static unsigned long aggregateBytes(void)
    {
      return sizeof(OcAggregate<T,AS>) ;
    }

    static unsigned long leafBytes(void)
    {
      return sizeof(OcLeaf<T,AS>) ;
    }

    unsigned long bytes(void) const
    {
      return bytesRecursive ( root_ ) + sizeof ( *this ) ;
    }

    int nodes(void) const
    {
      return nodesRecursive(root_);
    }

    int nodesAtSize(int size) const
    {
      return nodesAtSizeRecursive ( size , size_ , root_ ) ;
    }

    // Mutators
    void setEmptyValue(const T & emptyValue)
    {
      emptyValue_ = emptyValue ;
    }

    void swap(OctantTree<T,AS> & o)
    {
      OcNode<T,AS> * a              ;
      int            t              ;
      t             = emptyValue_   ;
      emptyValue_   = o.emptyValue_ ;
      o.emptyValue_ = t             ;
      t             = size_         ;
      size_         = o.size_       ;
      o.size_       = t             ;
      a             = root_         ;
      root_         = o.root_       ;
      o.root_       = a             ;
    }

    OctantTree<T,AS> & operator = (OctantTree<T,AS> o)
    {
      swap ( o )   ;
      return *this ;
    }

    T & operator() (int x,int y,int z)
    {
      OcAssert( x >= 0 && x < size_ ) ;
      OcAssert( y >= 0 && y < size_ ) ;
      OcAssert( z >= 0 && z < size_ ) ;
      OcNode<T,AS> ** n    = &root_   ;
      int             size = size_    ;
      while ( size != aggregateSize_ ) {
        if (!*n) {
          *n = (OcNode<T,AS> *)new OcBranch<T,AS>() ;
        } else
        if ( (*n)->type() == BranchNode ) {
          size /= 2;
          n = &reinterpret_cast<OcBranch<T,AS>*>(*n)->
               child(!!(x & size), !!(y & size), !!(z & size) ) ;
        } else {
          return reinterpret_cast<OcLeaf<T,AS>*>(*n)->value() ;
        }
      }
      if (!*n)                                                  {
        *n = new OcAggregate<T,AS>(emptyValue_) ;
      }
      --size;
      return reinterpret_cast<OcAggregate<T,AS>*>(*n)->value(x & size,y & size,z & size) ;
    }

    const T & operator() (int x,int y,int z) const
    {
      return at ( x , y , z ) ;
    }

    const T & at(int x,int y,int z) const
    {
      OcAssert ( x >= 0 && x < size_ )                             ;
      OcAssert ( y >= 0 && y < size_ )                             ;
      OcAssert ( z >= 0 && z < size_ )                             ;
      OcNode<T,AS> * n    = root_                                  ;
      int      size = size_                                        ;
      while ( size != aggregateSize_ )                             {
        if (!n) return emptyValue_ ; else
        if ( n->type() == BranchNode )                             {
          size /= 2                                                ;
          n = reinterpret_cast<OcBranch<T,AS>*>(n)->
              child ( !!(x & size) , !!(y & size) , !!(z & size) ) ;
        } else                                                     {
          OcAssert( n->type() == LeafNode )                        ;
          return reinterpret_cast<OcLeaf<T,AS>*>(n)->value()       ;
        }                                                          ;
      }                                                            ;
      if (!n) return emptyValue_                                   ;
      --size                                                       ;
      return reinterpret_cast<OcAggregate<T,AS>*>(n)->
             value ( x & size , y & size , z & size )              ;
    }

    void set(int x,int y,int z,const T & value)
    {
      if ( value != emptyValue() ) {
        (*this)(x,y,z) = value     ;
      } else erase(x,y,z)          ;
    }

    void erase(int x,int y,int z)
    {
      OcAssert       ( x >= 0 && x < size_        ) ;
      OcAssert       ( y >= 0 && y < size_        ) ;
      OcAssert       ( z >= 0 && z < size_        ) ;
      eraseRecursive ( &root_ , size_ , x , y , z ) ;
    }

    OcMatrix<T> zSlice( int z ) const
    {
      OcAssert          ( z >= 0 && z < size_                   ) ;
      OcMatrix<T> slice ( size_ , size_                         ) ;
      zSliceRecursive   ( slice , root_ , size_ , 0 , 0 , 0 , z ) ;
      return slice                                                ;
    }

    void writeBinary(std::ostream & out) const
    {
      if ( !root_ ) {
        static const char zero = 0;
        out.write( &zero, 1 );
      } else {
        static const char one = 1;
        out.write( &one, 1 );
        writeBinaryRecursive( out, root() );
      }
      out.write( reinterpret_cast<const char*>(&emptyValue_), sizeof(T  ) ) ;
      out.write( reinterpret_cast<const char*>(&size_      ), sizeof(int) ) ;
    }

    void readBinary(std::istream & in)
    {
      OctantTree<T,AS> tmp(0)                                           ;
      char             root                                             ;
      in . read ( &root, 1 )                                            ;
      if (root) readBinaryRecursive ( in , &tmp.root_ )                 ;
      in.read( reinterpret_cast<char*>(&tmp.emptyValue_), sizeof(T  ) ) ;
      in.read( reinterpret_cast<char*>(&tmp.size_      ), sizeof(int) ) ;
      if ( in.good() ) swap(tmp)                                        ;
    }

  protected:

          OcNode<T,AS> * & root(void)
    {
      return root_;
    }

    const OcNode<T,AS> *   root(void) const
    {
      return root_;
    }

    static void deleteNode(OcNode<T,AS> ** node)
    {
      OcAssert ( node )                                      ;
      if (*node)                                             {
        if ( (*node)->type() == BranchNode )                 {
          delete reinterpret_cast<OcBranch   <T,AS>*>(*node) ;
        } else
        if ( (*node)->type() == AggregateNode )              {
          delete reinterpret_cast<OcAggregate<T,AS>*>(*node) ;
        } else {
          OcAssert( (*node)->type() == LeafNode )            ;
          delete reinterpret_cast<OcLeaf     <T,AS>*>(*node) ;
        }                                                    ;
        *node = 0                                            ;
      }
    }

  private:

    void eraseRecursive ( OcNode<T,AS> ** node, int size, int x, int y, int z )
    {
      OcAssert(node)                                                    ;
      if ( !*node ) return                                              ;
      if ( size != aggregateSize_ )                                     {
        if ( (*node)->type() == BranchNode )                            {
          size /= 2                                                     ;
          OcBranch<T,AS> * b = reinterpret_cast<OcBranch<T,AS>*>(*node) ;
          eraseRecursive                                                (
            &b->child(!!(x & size), !!(y & size), !!(z & size))         ,
            size                                                        ,
            x                                                           ,
            y                                                           ,
            z                                                         ) ;
          for ( int i = 0; i < 8; ++i )                                 {
            if ( b->child(i) ) return                                   ;
          }                                                             ;
          deleteNode ( node )                                           ;
        } else
        if ( reinterpret_cast<OcLeaf<T,AS>*>(*node)->value() == emptyValue_) {
          deleteNode ( node )                                           ;
        } else                                                          {
          OcBranch<T,AS> * b = new OcBranch<T,AS>()                     ;
          size /= 2                                                     ;
          int childIndex = ( x & size ? 1 : 0 )
                         | ( y & size ? 2 : 0 )
                         | ( z & size ? 4 : 0 )                         ;
          const T & value = reinterpret_cast<OcLeaf<T,AS>*>(*node)->value() ;
          for ( int i = 0; i < 8; ++i )                                 {
            if ( i == childIndex ) continue                             ;
            if ( size == aggregateSize_ )                               {
              b->child(i) = (OcNode<T,AS> *)new OcLeaf<T,AS>(value)     ;
            } else                                                      {
              b->child(i) = (OcNode<T,AS> *)new OcAggregate<T,AS>(value);
            }                                                           ;
          }                                                             ;
          deleteNode ( node )                                           ;
          *node = b                                                     ;
           node = &b->child(childIndex)                                 ;
        }                                                               ;
      } else                                                            {
        --size                                                          ;
        OcAggregate<T,AS> * a = reinterpret_cast<OcAggregate<T,AS>*>(*node) ;
        a -> setValue ( x & size , y & size , z & size , emptyValue_ )  ;
        for ( int i = 0; i < AS * AS * AS ; ++i )                       {
          if ( a->value(i) != emptyValue_ ) return                      ;
        }                                                               ;
        deleteNode ( node )                                             ;
      }                                                                 ;
    }

    static unsigned long bytesRecursive(const OcNode<T,AS> * node)
    {
      if ( !node ) return 0             ;
      unsigned long b = 0               ;
      switch ( node->type() )           {
        case BranchNode                 :
          b = sizeof(OcBranch<T,AS>)    ;
          for ( int i = 0; i < 8; ++i ) {
            b += bytesRecursive(reinterpret_cast<const OcBranch<T,AS>*>(node)->child(i) ) ;
          }                             ;
        break                           ;
        case LeafNode                   :
          b = sizeof(OcLeaf<T,AS>)      ;
        break                           ;
        case AggregateNode              :
          b = sizeof(OcAggregate<T,AS>) ;
        break                           ;
      }                                 ;
      return b                          ;
    }

    static int nodesRecursive(const OcNode<T,AS> * node)
    {
      if ( !node ) return 0                     ;
      int n = 1                                 ;
      if ( node->type() == BranchNode )         {
        for ( int i = 0; i < 8; ++i )           {
          n += nodesRecursive(reinterpret_cast<const OcBranch<T,AS>*>(node)->child(i)) ;
        }                                       ;
      }                                         ;
      return n                                  ;
    }

    static int nodesAtSizeRecursive(int targetSize,int size,OcNode<T,AS> * node)
    {
      if (node)                                                              {
        if ( size == targetSize ) return 1                                   ;
        if ( node->type() == BranchNode )                            {
          int sum = 0                                                        ;
          for ( int i = 0; i < 2; ++i )                                      {
            for ( int j = 0; j < 2; ++j )                                    {
              for ( int k = 0; k < 2; ++k )                                  {
                sum += nodesAtSizeRecursive                                  (
                         targetSize                                          ,
                         size/2                                              ,
                         reinterpret_cast<OcBranch<T,AS>*>(node)->child(k,j,i)
                       )                                                     ;
              }                                                              ;
            }                                                                ;
          }                                                                  ;
          return sum                                                         ;
        }                                                                    ;
      }                                                                      ;
      return 0                                                               ;
    }

    void zSliceRecursive                (
           OcMatrix<T>          slice   ,
           const OcNode<T,AS> * node    ,
           int                  size    ,
           int                  x       ,
           int                  y       ,
           int                  z       ,
           int                  targetZ ) const
    {
      if (!node) {
        for ( int i = 0; i < slice.M(); ++i ) {
          for ( int j = 0; j < slice.N(); ++j ) {
            slice(i,j) = emptyValue_;
          }
        }
      } else
      if ( node->type() == BranchNode ) {
        size /= 2;
        for ( int i = 0; i < 2; ++i ) {
          for ( int j = 0; j < 2; ++j ) {
            zSliceRecursive (
              slice.subarray(
                i*size,
                j*size,
                (i+1)*size,
                (j+1)*size),
                reinterpret_cast<const OcBranch<T,AS>*>(node)->child(j,i,!!(targetZ & size)) ,
              size, x, y, z, targetZ
            ) ;
          }
        }
      } else
      if ( node->type() == AggregateNode ) {
        for ( int i = 0; i < slice.M(); ++i ) {
          for ( int j = 0; j < slice.N(); ++j ) {
            slice(i,j) = reinterpret_cast<const OcAggregate<T,AS>*>(node)->value(j,i,targetZ - z & (size-1) ) ;
          }
        }
      } else {
        OcAssert( node->type() == LeafNode );
        for ( int i = 0; i < slice.M(); ++i ) {
          for ( int j = 0; j < slice.N(); ++j ) {
            slice(i,j) = reinterpret_cast<const OcLeaf<T,AS>*>(node)->value() ;
          }
        }
      }
    }

    static void writeBinaryRecursive(std::ostream & out,const OcNode<T,AS> * node)
    {
      OcAssert ( node )                              ;
      if ( !out.good() ) return                      ;
      char type = node->type()                       ;
      out.write( &type, 1 )                          ;
      switch (type)                                  {
        case BranchNode                              :
        {
          const OcBranch<T,AS> * b = reinterpret_cast<const OcBranch<T,AS>*>(node);
          char                   children = 0        ;
          for ( int i = 0; i < 8; ++i )              {
            children |= ( b->child(i) != 0 ) << i    ;
          }                                          ;
          out.write( &children, 1 )                  ;
          for ( int i = 0; i < 8; ++i )              {
            if ( b->child(i) )                       {
              writeBinaryRecursive(out, b->child(i)) ;
            }                                        ;
          }                                          ;
        }                                            ;
        break                                        ;
        case AggregateNode                           :
          out.write( reinterpret_cast<const char*>   (
                    &reinterpret_cast<const OcAggregate<T,AS>*>(node)->value(0,0,0)
                    ),AS*AS*AS*sizeof(T)           ) ;
        break                                        ;
        case LeafNode                                :
           out.write( reinterpret_cast<const char*>  (
                     &reinterpret_cast<const OcLeaf<T,AS>*>(node)->value()
                     ),sizeof(T)                   ) ;
        break                                        ;
      }                                              ;
    }

    static void readBinaryRecursive(std::istream & in,OcNode<T,AS> ** node)
    {
      OcAssert(node) ;
      if ( !in.good() ) return ;
      char type;
      in . read ( &type , 1 ) ;
      switch (type) {
        case BranchNode:
        {
          OcBranch<T,AS> * b = new OcBranch<T,AS>()           ;
          *node = b                                           ;
          char children                                       ;
          in . read ( &children , 1 )                         ;
          for ( int i = 0; i < 8; ++i )                       {
            if ( children & (1 << i) )                        {
              readBinaryRecursive(in,&b->child(i))            ;
            }                                                 ;
          }                                                   ;
        }                                                     ;
        break                                                 ;
        case AggregateNode                                    :
        {
          OcAggregate<T,AS> * a = new OcAggregate<T,AS>(T(0)) ;
          *node = a                                           ;
          in.read(reinterpret_cast<char*>(&a->value(0,0,0)),AS*AS*AS*sizeof(T)) ;
        }                                                     ;
        break                                                 ;
        case LeafNode                                         :
        {
          OcLeaf<T,AS> * l = new OcLeaf<T,AS>(T(0))           ;
          *node = l                                           ;
          in.read( reinterpret_cast<char*>(&l->value()),sizeof(T)) ;
        }                                                     ;
        break                                                 ;
      }                                                       ;
    }

};

class Q_DISCRETE_EXPORT OcTree : public Graph
{
  public:

    Nodes root ;

    explicit OcTree (void) ;
    virtual ~OcTree (void) ;

  protected:

  private:

};

}

QT_END_NAMESPACE

Q_DECLARE_METATYPE(N::SetOperator)
Q_DECLARE_METATYPE(N::SetsAlgebra)
Q_DECLARE_METATYPE(N::Node)
Q_DECLARE_METATYPE(N::Nodes)
Q_DECLARE_METATYPE(N::Relation)
Q_DECLARE_METATYPE(N::Relations)
Q_DECLARE_METATYPE(N::Binary)
Q_DECLARE_METATYPE(N::Graph)
Q_DECLARE_METATYPE(N::Tree)
Q_DECLARE_METATYPE(N::OcTree)

#endif
