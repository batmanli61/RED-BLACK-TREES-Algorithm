#include<iostream>
#include <stdlib.h>
#include<ctime>

using namespace std;
//1. Completely describe one of the following algorithms, 
//   including initialization, searching, insertion, deletion operations AVL tree Red-Black tree
//2. Implement the selected algorithm using C++ and test it using a set of data 


#define __TEST__

template <class T>  //class T ad�nda �ablon olu�turuldu
class rbtree; //class tan�mland�

template <class T> //�ablon olu�turuldu  
struct _rbTreeNode{  //rbTreeNode ad�nda a�a� yap�s� olu�turuldu
	friend class rbtree<T>; //olu�turdu�umuz class� arkada� class yapt�k
	T getkey(){         //sablonu olu�turdu�umuz classtan deger al�p d�nd�rd�
		return key;
	}
private:   //private deger tan�mlamalar� olacak
	T key;
	_rbTreeNode<T> *left; //a�ac�m�zz�n sol pointer�
	_rbTreeNode<T> *right; //sa� pointere
	_rbTreeNode<T> *p;     //bir de p ad�nda pointer tan�m� yap�yoruz
	bool color;			//renk olarak  true == red  &&  false == black
};

template <class T>//�ablon olu�turduk
class rbtree{
//rbtree ad�nda class olu�turduk
public:  //public olarak degerler olu�turulacak
	rbtree()     
		:nil( new _rbTreeNode<T> ), root( nil ) { nil->left = 0; nil->p = 0; nil->right = 0; nil->color = false; }
		//deger y�nlendirmesi yap�l�yor
	void insert( T key );//ekliyor
	 _rbTreeNode<T> * search( T key );//degeri arat�yor agacta
	void erase( T key ); //sildiriyor 

#ifdef __TEST__  //olduysa(�artlar)
	void display(); //dha sonra sergiliyor
#endif//�art yap�s� kullan�l�yor

private:// private deger tan�mlamas�

#ifdef __TEST__//�art olduysa 
	void _display( _rbTreeNode<T> * ); //rbTreeNode u sergilesin
#endif   //olduktan sonra 


	void leftRotate(  _rbTreeNode<T> * );//sol tarafa g�t�r
	void rightRotate(  _rbTreeNode<T> * ); //sag tarafa g�t�r
	
	void rbDelete(  _rbTreeNode<T> * ); //agactaki hedefi sil
	void rbDeleteFixup(  _rbTreeNode<T> * ); //d�ng�de bir bos degere g�re(while ile )
	
	void rbInsertFixup( _rbTreeNode<T> * );//d�ng� �art�na g�re ekleme
	
	_rbTreeNode<T>* treeSuccessor(  _rbTreeNode<T> * );//agac mimarisi
	
	_rbTreeNode<T> *nil;	//�ablon pointere
	_rbTreeNode<T> *root;   // �ablon pointere

};

template <class T>  //class �ablonu olu�turuldu
void rbtree<T>::insert( T key ){   //agaca ekleme yapt�k
    //rbTreeNode t�r�nde T class tipinde pointer tan�mland�
	_rbTreeNode<T>* t = new _rbTreeNode<T>; 
	t->key = key;   //key iyi tnin keyine atad�k
	_rbTreeNode<T>* x = root; //rootu x pointera atad�k
	_rbTreeNode<T>* y = nil;//nil i y pointera atad�k
	while( x != nil ){  //nil x ten farkl� ise 
		y = x; //y=x olsun
		if( key < x->key )  //key x teki key den k���k ise
			x = x->left; //x solunu x e ata
		else //de�ilse
			x = x->right; //x e x in sa��n� ata
	}
	t->p = y;  //y yi tnin p pointer�na ata
	if( y == nil ) //y nile e�it ise 
		root = t; //t yi root a ekle
	else{//de�ilse
		if ( t->key < y->key )//tnin keyi ynin keyinden k���kse
			y->left = t;//y nin soluna t yi ata
		else//degilse
			y->right = t;//y nin sa��na t yi ata
	}
	t->left = nil; //t nin soluna nili ata
	t->right = nil;//t nin sa��na da nili ata
	t->color = true;//t nin rengi k�rm�z� olsun
	rbInsertFixup( t ); //t yi ekle (rbye)
}

template <class T>//t class�na g�re �ablon olu�turuldu
_rbTreeNode<T> * rbtree<T>::search( T key ){//a�ac i�in arama yap�lacak
	_rbTreeNode<T> * x = root; //tan�mlanan x pointer�na root atan�r
	while( x != nil && key != x->key ){//�art sa�lad�g� s�rece d�ng� devam
		if( key < x->key )//key x in keyinden k���kse
			x = x->left;//x in solunu x e ata
		else//de�ilse
			x = x->right;//x in sa��n� x e ata
	}
	return x;//x i d�nd�r
}

template <class T>//�ablon olu�turuldu
void rbtree<T>::erase( T key ){  //silme islemi icin
     //x pointer�n� ve keyi kullanarak arama
	_rbTreeNode<T> * x = search( key );
	if( x != nil )//x nilden farkl� ise
		rbDelete( x );//x i agatan sil
}

template <class T>//�ablon olu�turuldu
//agac mimarisi i�lemleri i�in diyebilirim(tan�mlamalar� yap�ld�)
_rbTreeNode<T> * rbtree<T>::treeSuccessor( _rbTreeNode<T> *x ){
	if( x->right != nil ){//x in sa�� nilden farkl�ysa
	//x in solu nilden farkl� oldugu s�rece d�ng� �al��s�n
		while( x->left != nil )
			x = x->left;//x in solunu x e atar
		return x;//x i d�nd�r�r
	}
	_rbTreeNode<T> * y = x->p;//agac�n y pointer�na x in p sini atar 
	while( y != nil && x == y->right ){//�art sa�lad�g� s�re d�ng� devam
		x = y;//y yi x e atar
		y = y->p;//ynin p pointer�n�  y ye atar
	}
	return y;//y yi d�nd�r�r
}

template <class T>//�ablon olu�turur
      //agacn sol yolu i�in tan�mlama ve fonksiyon
void rbtree<T>::leftRotate( _rbTreeNode<T> *x ){   // x->right != nil
	_rbTreeNode<T> * y = x->right;//x in sa��n� y ye atar
	if( x->p == nil )//x in p si nile esit olursa
		root = y;//y yi root'a ata
	else{//degilse
	   //xin p pointer�n�n solunu x e ata
		if( x == x->p->left )
			x->p->left = y;//y yi x->p->soluna ata
		else//degilse
			x->p->right = y;
			//x in p pointer�n�n sa��na y yi ata
	}
	y->p = x->p;//x in p sini y nin p sine ata
	x->right = y->left;//y nin solunu x in sa��na ata
	y->left->p = x; //x i ynin solunun p sine ata
	y->left = x;//y nin soluna x i ata
	x->p = y;//x in p sine y yi ata
}

template <class T>  //�ablon olu�turuldu
   //sa� rotasyon i�in tan�m ve fonksiyon
void rbtree<T>::rightRotate( _rbTreeNode<T> *x ){
	//y pointer�na x in solu atan�r
	_rbTreeNode<T> * y = x->left;
	if( x->p == nil )//x in p si nile esitse
		root = y;//y yi root a ata
	else{//degilse
		if( x->p->left == x )//x in p sinin solu x ise
			x->p->left = y;//y yi x->p->soluna ata
		else//degilse
			x->p->right = y;//x in psinin sa��na y yi ata
	}
	y->p = x->p;//x in p sini y nin p sine ata
	x->left = y->right;//y nin sa��n� x in soluna ata
	y->right->p = x;//y nin sa��n�n psine x i ata
	y->right = x;//y nin sa��na x i ata
	x->p = y;//x in p sine y yi ata
}


template <class T>//�ablon olu�turuldu
//insertFixup i�lemi i�n tan�mlamalar yap�ld�
void rbtree<T>::rbInsertFixup( _rbTreeNode<T> *z ){
	while( z->p->color  == true ){
		//znin psinin rengi k�rm�z� ise d�ng� devam etsin
		if( z->p == z->p->p->left ){//�art sa�larsa
			_rbTreeNode<T> * y = z->p->p->right; //y ye ata
			if( y->color == true ){
				//y nin rengi k�rm�z� ise
				z->p->color = false;//znin psinin rengi siyah olsun
				y->color = false;//y nin rengi siyah olsun
				z->p->p->color = true;//k�rm�z� olsun
				z = z->p->p;//z ye znin psinin p sini ata
			}
			else{//degilse
				if( z == z->p->right ){//�art� sa�larsa
					z = z->p;//z ye zni psini ata
					leftRotate( z ); 
					//z yi sol rotasyona g�nder(fonksiyona)
				}
				z->p->color = false;//znin psinin rengi siyah olsun
				z->p->p->color = true;//k�rm�z� olsun
				z->p->right->color = false;//siyah olsun
				rightRotate( z->p->p );//sa� rotasyon fonksiyonuna g�nder
			}
		}
		else{//degilse
			_rbTreeNode<T> * y = z->p->p->left;// y pointera atama yap
			if( y->color == true ){//ynin rengi k�rm�z� ise
				z->p->color = false;//znin psinin rengi siyah olsun
				y->color = false;//ynin rengi siyah olsun 
				z->p->p->color = true;//k�rm�z� olsun
				z = z->p->p;//z ye znin psinin psini ata
			}
			else{//degilse
				if( z == z->p->left ){//�art sa�lan�yorsa
					z = z->p;//znin psini z ye ata
					rightRotate( z );
					// z yi sa� rotasyon fonksiyonuna g�nder
				}
				z->p->color = false;//znin psinin rengi siyah olsun
				z->p->p->color = true;//rengi k�rm�z� olsun
				z->p->left->color = false;//siyah olsun
				leftRotate( z->p->p );//sol rotasyon fonksiyonuna g�nder
			}
		}
	}
	root->color = false;//root un rengi siyah olsun
}


template <class T>//�ablon olu�turur
//silme islemi i�in olu�turulmu� fonksiyonumuz
void rbtree<T>::rbDelete( _rbTreeNode<T> *z ){
	_rbTreeNode<T> * x = nil;//nili x e ata
	_rbTreeNode<T> * y = nil;//nili y ye ata
	if( z->left == nil || z->right == nil )//�art sa�larsa
		y = z;//z yi y ye ata
	else//de�ilse
		y = treeSuccessor( z );//znin mimarisini y ye atar
	if( y->left != nil )//y nin solu nilden farkl�ysa
		x = y->left;//y nin solunu x e ata
	else//degilse
		x = y->right;//y nin sa��n� x e ata
	x->p = y->p; //ynin psini x in psine ata
	if( y->p == nil )//ynin psi nile e�itse
		root = x; //xi roota ata
	else{//degilse
		if( y == y->p->left )//�art� sa�l�yorsa
			y->p->left = x;//x i ynin psinin soluna ata
		else//degilse
			y->p->right = x;//y nin psinin sa��na x ata
	}
	if( y != z )//y, z den farkl�ysa 
		z->key = y->key;//y nin keyini znin keyine ata
	if( y->color == false )//ynin rengi siyah ise
		rbDeleteFixup( x );//agactan x i sil                                     //fonkisyon olup olmad�g�n� kontrol et
	delete y;//y yi sil
}

template <class T>//�ablon tan�mland�
     //atactan silme i�lemi i�in tan�mlamalar
void rbtree<T>::rbDeleteFixup(_rbTreeNode<T> * x){
	//x roottan farkl� ve rengi siyah ise d�ng� devam etsin
	while( x != root && x->color == false ){
		_rbTreeNode<T> * w = 0;//pointer tan�mland� atama oldu
		if( x->p->left == x ){//�art sa�larsa
			w = x->p->right;//x in psinin sa��n� w ya ata
			if( w->color == true ){//wnin rengi k�rm�z� ise
				w->color = false;//w nun rengi siyah olsun
				x->p->color = true;//k�rm�z� olsun
				leftRotate( x->p );
				//x in psini sol rotasyona g�nder(fonksiyona)
				w = x->p->right;//x in psinin sa��n� w ya ata
			}
			//w nun solunun rengi siyah ve sa��n�n rengi siyah ise
			if( w->left->color == false && w->right->color == false ){
				w->color = true;//w nun rengi k�rm�z� olsun
				x = x->p; //x in p pointer�n� x e ata
			}
			else{//degilse
				if( w->right->color == false )
				{//w nun sa��n�n rengi siyah ise
					w->left->color = false;//solunun rengi siyah olsun
					w->color = true;//w nun rengi k�rm�z� olsun
					rightRotate( w );//w yu sa� rotayona g�nder
					w = x->p->right;//x in psinin sa��n� w ata
				}
				//x in psinin rengini w nun rengine ata
				w->color = x->p->color;
				x->p->color = false;//x in psinin rengi siyah olsun
				w->right->color = false;//w sa��n�n rengi siyah olsun
				leftRotate( x->p );//sol rotasyona x->p yi g�nder
				x = root;//root'u x'e ata
			}
		}
		else{//degilse
			w = x->p->left;//w ya x in psinin solunu ata
			if( w->color == true )
			{//w nun rengi k�rm�z� ise
				w->color = false;//w nun rengi siyah olsun
				x->p->color = true;//x in psinin rengi k�rm�z� olsun
				rightRotate( x->p );//x->p yi sa� rotasyona g�nder(fonksiyona)
				w = x->p->left;//x in psinin solunu w ya ata
			}
			if( w->right->color == false && w->left->color == false )
			{//w nun sa��n�n rengi siyah ve solunun rengi siyah ise
				w->color = true;//w nun rengi k�rm�z� olsun
				x = x->p;//x in psini x e ata
			}
			else{//degilse
				if( w->left->color == false )
				{//w nun solunun rengi siyah ise
					w->right->color = false;//sa��n� rengi siyah olsun
					w->color = true;//w nun rengi k�rm�z� olsun
					leftRotate( w );//w yu sol rotasyona g�nder(fonksiyona)
					w = x->p->left;//x ini psinin solunu w ya ata
				}
				w->color = x->p->color;//x in psinin rengini w rengine ata
				x->p->color = false;//x in psinin rengi siyah olsun
				w->left->color = false;//w nun solunun rengi siyah olsun
				rightRotate( x->p );//sa� rotasyona x->p yi g�nder
				x = root;//root'u x'e ata
			}
		}
	}
	x->color = false;//x in rengi siyah olsun
}

#ifdef __TEST__//test olduysa

template <class T>//class t t�r�nde �ablon olu�tu
void rbtree<T>::display(){//sergilemek i�in
	if( root != nil )//root nil den farkl� ise
		_display( root );//root'u g�ster
	else//degilse 
		cout<<"agac bos"<<endl;//bos yaz
}

template <class T>//�ablon olu�turuldu
void rbtree<T>::_display( _rbTreeNode<T> * x ){
	if( x->left != nil )//x in solu nilde farkl� ise
		_display( x->left );//x in solunu g�ster
	if( x != nil ){//x nilden farkl� ise
		cout << x->key << ' ';//x in keyini yaz
		if( x->color == true )//x in rengi k�rm�z� ise
			cout << "KIRMIZI ";//k�rm�z� yazd�r
		else//degilse
			cout << "SIYAH ";//siyah yazd�r
		if( x->p != nil )//x in psi nilden farkl� ise
			cout << x->p->key << ' ';//yazd�r
		else//degilse
			cout << "BOS ";//bos yazd�r
		if( x->left != nil )//x in solu nilden farkl� ise
			cout << x->left->key << ' ';//yazd�r
		else//degilse
			cout << "BOS ";//bos yazd�r
		if( x->right != nil )//x in sa�� nilden farkl� ise
			cout << x->right->key << ' ';//yazd�r
		else//degilse
			cout << "BOS ";//bos yazd�r
	}
	cout << endl;//bir sat�r asag� indir
	if( x->right != nil )//x in sa�� nilden farl� ise
		_display( x->right );//x in sa��n� g�r�nt�le
}

#endif

const int N = 15;//N sabiti tan�mland�

int main(){//ana blok
     //int veri t�r�nde test ad�nda agac tan�t�ld�
	rbtree<int> test;
	srand((unsigned)time(NULL));//rastgele deger d�nd�rme
	srand( rand() );//rastgele deger d�nd�rme
	srand( rand() );//rastgele deger d�nd�rme
	for( int i = 0; i != N; ++i ){//d�ng�
		test.insert( rand() );//insert blo�una gider
	}
	int a;//degisken tan�mlama
	test.display();//agac� g�r�nt�leme
	while( cin >> a ){//a degeri girilirdiyse
		test.erase( a );//a degerini agactan silip
		test.display();//agac� yeniden g�sterir
	}
	return 0;
}
