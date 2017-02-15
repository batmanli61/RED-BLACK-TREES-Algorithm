#include<iostream>
#include <stdlib.h>
#include<ctime>

using namespace std;
//1. Completely describe one of the following algorithms, 
//   including initialization, searching, insertion, deletion operations AVL tree Red-Black tree
//2. Implement the selected algorithm using C++ and test it using a set of data 


#define __TEST__

template <class T>  //class T adýnda þablon oluþturuldu
class rbtree; //class tanýmlandý

template <class T> //þablon oluþturuldu  
struct _rbTreeNode{  //rbTreeNode adýnda aðaç yapýsý oluþturuldu
	friend class rbtree<T>; //oluþturduðumuz classý arkadaþ class yaptýk
	T getkey(){         //sablonu oluþturduðumuz classtan deger alýp döndürdü
		return key;
	}
private:   //private deger tanýmlamalarý olacak
	T key;
	_rbTreeNode<T> *left; //aðacýmýzzýn sol pointerý
	_rbTreeNode<T> *right; //sað pointere
	_rbTreeNode<T> *p;     //bir de p adýnda pointer tanýmý yapýyoruz
	bool color;			//renk olarak  true == red  &&  false == black
};

template <class T>//þablon oluþturduk
class rbtree{
//rbtree adýnda class oluþturduk
public:  //public olarak degerler oluþturulacak
	rbtree()     
		:nil( new _rbTreeNode<T> ), root( nil ) { nil->left = 0; nil->p = 0; nil->right = 0; nil->color = false; }
		//deger yönlendirmesi yapýlýyor
	void insert( T key );//ekliyor
	 _rbTreeNode<T> * search( T key );//degeri aratýyor agacta
	void erase( T key ); //sildiriyor 

#ifdef __TEST__  //olduysa(þartlar)
	void display(); //dha sonra sergiliyor
#endif//þart yapýsý kullanýlýyor

private:// private deger tanýmlamasý

#ifdef __TEST__//þart olduysa 
	void _display( _rbTreeNode<T> * ); //rbTreeNode u sergilesin
#endif   //olduktan sonra 


	void leftRotate(  _rbTreeNode<T> * );//sol tarafa götür
	void rightRotate(  _rbTreeNode<T> * ); //sag tarafa götür
	
	void rbDelete(  _rbTreeNode<T> * ); //agactaki hedefi sil
	void rbDeleteFixup(  _rbTreeNode<T> * ); //döngüde bir bos degere göre(while ile )
	
	void rbInsertFixup( _rbTreeNode<T> * );//döngü þartýna göre ekleme
	
	_rbTreeNode<T>* treeSuccessor(  _rbTreeNode<T> * );//agac mimarisi
	
	_rbTreeNode<T> *nil;	//þablon pointere
	_rbTreeNode<T> *root;   // þablon pointere

};

template <class T>  //class þablonu oluþturuldu
void rbtree<T>::insert( T key ){   //agaca ekleme yaptýk
    //rbTreeNode türünde T class tipinde pointer tanýmlandý
	_rbTreeNode<T>* t = new _rbTreeNode<T>; 
	t->key = key;   //key iyi tnin keyine atadýk
	_rbTreeNode<T>* x = root; //rootu x pointera atadýk
	_rbTreeNode<T>* y = nil;//nil i y pointera atadýk
	while( x != nil ){  //nil x ten farklý ise 
		y = x; //y=x olsun
		if( key < x->key )  //key x teki key den küçük ise
			x = x->left; //x solunu x e ata
		else //deðilse
			x = x->right; //x e x in saðýný ata
	}
	t->p = y;  //y yi tnin p pointerýna ata
	if( y == nil ) //y nile eþit ise 
		root = t; //t yi root a ekle
	else{//deðilse
		if ( t->key < y->key )//tnin keyi ynin keyinden küçükse
			y->left = t;//y nin soluna t yi ata
		else//degilse
			y->right = t;//y nin saðýna t yi ata
	}
	t->left = nil; //t nin soluna nili ata
	t->right = nil;//t nin saðýna da nili ata
	t->color = true;//t nin rengi kýrmýzý olsun
	rbInsertFixup( t ); //t yi ekle (rbye)
}

template <class T>//t classýna göre þablon oluþturuldu
_rbTreeNode<T> * rbtree<T>::search( T key ){//aðac için arama yapýlacak
	_rbTreeNode<T> * x = root; //tanýmlanan x pointerýna root atanýr
	while( x != nil && key != x->key ){//þart saðladýgý sürece döngü devam
		if( key < x->key )//key x in keyinden küçükse
			x = x->left;//x in solunu x e ata
		else//deðilse
			x = x->right;//x in saðýný x e ata
	}
	return x;//x i döndür
}

template <class T>//þablon oluþturuldu
void rbtree<T>::erase( T key ){  //silme islemi icin
     //x pointerýný ve keyi kullanarak arama
	_rbTreeNode<T> * x = search( key );
	if( x != nil )//x nilden farklý ise
		rbDelete( x );//x i agatan sil
}

template <class T>//þablon oluþturuldu
//agac mimarisi iþlemleri için diyebilirim(tanýmlamalarý yapýldý)
_rbTreeNode<T> * rbtree<T>::treeSuccessor( _rbTreeNode<T> *x ){
	if( x->right != nil ){//x in saðý nilden farklýysa
	//x in solu nilden farklý oldugu sürece döngü çalýþsýn
		while( x->left != nil )
			x = x->left;//x in solunu x e atar
		return x;//x i döndürür
	}
	_rbTreeNode<T> * y = x->p;//agacýn y pointerýna x in p sini atar 
	while( y != nil && x == y->right ){//þart saðladýgý süre döngü devam
		x = y;//y yi x e atar
		y = y->p;//ynin p pointerýný  y ye atar
	}
	return y;//y yi döndürür
}

template <class T>//þablon oluþturur
      //agacn sol yolu için tanýmlama ve fonksiyon
void rbtree<T>::leftRotate( _rbTreeNode<T> *x ){   // x->right != nil
	_rbTreeNode<T> * y = x->right;//x in saðýný y ye atar
	if( x->p == nil )//x in p si nile esit olursa
		root = y;//y yi root'a ata
	else{//degilse
	   //xin p pointerýnýn solunu x e ata
		if( x == x->p->left )
			x->p->left = y;//y yi x->p->soluna ata
		else//degilse
			x->p->right = y;
			//x in p pointerýnýn saðýna y yi ata
	}
	y->p = x->p;//x in p sini y nin p sine ata
	x->right = y->left;//y nin solunu x in saðýna ata
	y->left->p = x; //x i ynin solunun p sine ata
	y->left = x;//y nin soluna x i ata
	x->p = y;//x in p sine y yi ata
}

template <class T>  //þablon oluþturuldu
   //sað rotasyon için taným ve fonksiyon
void rbtree<T>::rightRotate( _rbTreeNode<T> *x ){
	//y pointerýna x in solu atanýr
	_rbTreeNode<T> * y = x->left;
	if( x->p == nil )//x in p si nile esitse
		root = y;//y yi root a ata
	else{//degilse
		if( x->p->left == x )//x in p sinin solu x ise
			x->p->left = y;//y yi x->p->soluna ata
		else//degilse
			x->p->right = y;//x in psinin saðýna y yi ata
	}
	y->p = x->p;//x in p sini y nin p sine ata
	x->left = y->right;//y nin saðýný x in soluna ata
	y->right->p = x;//y nin saðýnýn psine x i ata
	y->right = x;//y nin saðýna x i ata
	x->p = y;//x in p sine y yi ata
}


template <class T>//þablon oluþturuldu
//insertFixup iþlemi içn tanýmlamalar yapýldý
void rbtree<T>::rbInsertFixup( _rbTreeNode<T> *z ){
	while( z->p->color  == true ){
		//znin psinin rengi kýrmýzý ise döngü devam etsin
		if( z->p == z->p->p->left ){//þart saðlarsa
			_rbTreeNode<T> * y = z->p->p->right; //y ye ata
			if( y->color == true ){
				//y nin rengi kýrmýzý ise
				z->p->color = false;//znin psinin rengi siyah olsun
				y->color = false;//y nin rengi siyah olsun
				z->p->p->color = true;//kýrmýzý olsun
				z = z->p->p;//z ye znin psinin p sini ata
			}
			else{//degilse
				if( z == z->p->right ){//þartý saðlarsa
					z = z->p;//z ye zni psini ata
					leftRotate( z ); 
					//z yi sol rotasyona gönder(fonksiyona)
				}
				z->p->color = false;//znin psinin rengi siyah olsun
				z->p->p->color = true;//kýrmýzý olsun
				z->p->right->color = false;//siyah olsun
				rightRotate( z->p->p );//sað rotasyon fonksiyonuna gönder
			}
		}
		else{//degilse
			_rbTreeNode<T> * y = z->p->p->left;// y pointera atama yap
			if( y->color == true ){//ynin rengi kýrmýzý ise
				z->p->color = false;//znin psinin rengi siyah olsun
				y->color = false;//ynin rengi siyah olsun 
				z->p->p->color = true;//kýrmýzý olsun
				z = z->p->p;//z ye znin psinin psini ata
			}
			else{//degilse
				if( z == z->p->left ){//þart saðlanýyorsa
					z = z->p;//znin psini z ye ata
					rightRotate( z );
					// z yi sað rotasyon fonksiyonuna gönder
				}
				z->p->color = false;//znin psinin rengi siyah olsun
				z->p->p->color = true;//rengi kýrmýzý olsun
				z->p->left->color = false;//siyah olsun
				leftRotate( z->p->p );//sol rotasyon fonksiyonuna gönder
			}
		}
	}
	root->color = false;//root un rengi siyah olsun
}


template <class T>//þablon oluþturur
//silme islemi için oluþturulmuþ fonksiyonumuz
void rbtree<T>::rbDelete( _rbTreeNode<T> *z ){
	_rbTreeNode<T> * x = nil;//nili x e ata
	_rbTreeNode<T> * y = nil;//nili y ye ata
	if( z->left == nil || z->right == nil )//þart saðlarsa
		y = z;//z yi y ye ata
	else//deðilse
		y = treeSuccessor( z );//znin mimarisini y ye atar
	if( y->left != nil )//y nin solu nilden farklýysa
		x = y->left;//y nin solunu x e ata
	else//degilse
		x = y->right;//y nin saðýný x e ata
	x->p = y->p; //ynin psini x in psine ata
	if( y->p == nil )//ynin psi nile eþitse
		root = x; //xi roota ata
	else{//degilse
		if( y == y->p->left )//þartý saðlýyorsa
			y->p->left = x;//x i ynin psinin soluna ata
		else//degilse
			y->p->right = x;//y nin psinin saðýna x ata
	}
	if( y != z )//y, z den farklýysa 
		z->key = y->key;//y nin keyini znin keyine ata
	if( y->color == false )//ynin rengi siyah ise
		rbDeleteFixup( x );//agactan x i sil                                     //fonkisyon olup olmadýgýný kontrol et
	delete y;//y yi sil
}

template <class T>//þablon tanýmlandý
     //atactan silme iþlemi için tanýmlamalar
void rbtree<T>::rbDeleteFixup(_rbTreeNode<T> * x){
	//x roottan farklý ve rengi siyah ise döngü devam etsin
	while( x != root && x->color == false ){
		_rbTreeNode<T> * w = 0;//pointer tanýmlandý atama oldu
		if( x->p->left == x ){//þart saðlarsa
			w = x->p->right;//x in psinin saðýný w ya ata
			if( w->color == true ){//wnin rengi kýrmýzý ise
				w->color = false;//w nun rengi siyah olsun
				x->p->color = true;//kýrmýzý olsun
				leftRotate( x->p );
				//x in psini sol rotasyona gönder(fonksiyona)
				w = x->p->right;//x in psinin saðýný w ya ata
			}
			//w nun solunun rengi siyah ve saðýnýn rengi siyah ise
			if( w->left->color == false && w->right->color == false ){
				w->color = true;//w nun rengi kýrmýzý olsun
				x = x->p; //x in p pointerýný x e ata
			}
			else{//degilse
				if( w->right->color == false )
				{//w nun saðýnýn rengi siyah ise
					w->left->color = false;//solunun rengi siyah olsun
					w->color = true;//w nun rengi kýrmýzý olsun
					rightRotate( w );//w yu sað rotayona gönder
					w = x->p->right;//x in psinin saðýný w ata
				}
				//x in psinin rengini w nun rengine ata
				w->color = x->p->color;
				x->p->color = false;//x in psinin rengi siyah olsun
				w->right->color = false;//w saðýnýn rengi siyah olsun
				leftRotate( x->p );//sol rotasyona x->p yi gönder
				x = root;//root'u x'e ata
			}
		}
		else{//degilse
			w = x->p->left;//w ya x in psinin solunu ata
			if( w->color == true )
			{//w nun rengi kýrmýzý ise
				w->color = false;//w nun rengi siyah olsun
				x->p->color = true;//x in psinin rengi kýrmýzý olsun
				rightRotate( x->p );//x->p yi sað rotasyona gönder(fonksiyona)
				w = x->p->left;//x in psinin solunu w ya ata
			}
			if( w->right->color == false && w->left->color == false )
			{//w nun saðýnýn rengi siyah ve solunun rengi siyah ise
				w->color = true;//w nun rengi kýrmýzý olsun
				x = x->p;//x in psini x e ata
			}
			else{//degilse
				if( w->left->color == false )
				{//w nun solunun rengi siyah ise
					w->right->color = false;//saðýný rengi siyah olsun
					w->color = true;//w nun rengi kýrmýzý olsun
					leftRotate( w );//w yu sol rotasyona gönder(fonksiyona)
					w = x->p->left;//x ini psinin solunu w ya ata
				}
				w->color = x->p->color;//x in psinin rengini w rengine ata
				x->p->color = false;//x in psinin rengi siyah olsun
				w->left->color = false;//w nun solunun rengi siyah olsun
				rightRotate( x->p );//sað rotasyona x->p yi gönder
				x = root;//root'u x'e ata
			}
		}
	}
	x->color = false;//x in rengi siyah olsun
}

#ifdef __TEST__//test olduysa

template <class T>//class t türünde þablon oluþtu
void rbtree<T>::display(){//sergilemek için
	if( root != nil )//root nil den farklý ise
		_display( root );//root'u göster
	else//degilse 
		cout<<"agac bos"<<endl;//bos yaz
}

template <class T>//þablon oluþturuldu
void rbtree<T>::_display( _rbTreeNode<T> * x ){
	if( x->left != nil )//x in solu nilde farklý ise
		_display( x->left );//x in solunu göster
	if( x != nil ){//x nilden farklý ise
		cout << x->key << ' ';//x in keyini yaz
		if( x->color == true )//x in rengi kýrmýzý ise
			cout << "KIRMIZI ";//kýrmýzý yazdýr
		else//degilse
			cout << "SIYAH ";//siyah yazdýr
		if( x->p != nil )//x in psi nilden farklý ise
			cout << x->p->key << ' ';//yazdýr
		else//degilse
			cout << "BOS ";//bos yazdýr
		if( x->left != nil )//x in solu nilden farklý ise
			cout << x->left->key << ' ';//yazdýr
		else//degilse
			cout << "BOS ";//bos yazdýr
		if( x->right != nil )//x in saðý nilden farklý ise
			cout << x->right->key << ' ';//yazdýr
		else//degilse
			cout << "BOS ";//bos yazdýr
	}
	cout << endl;//bir satýr asagý indir
	if( x->right != nil )//x in saðý nilden farlý ise
		_display( x->right );//x in saðýný görüntüle
}

#endif

const int N = 15;//N sabiti tanýmlandý

int main(){//ana blok
     //int veri türünde test adýnda agac tanýtýldý
	rbtree<int> test;
	srand((unsigned)time(NULL));//rastgele deger döndürme
	srand( rand() );//rastgele deger döndürme
	srand( rand() );//rastgele deger döndürme
	for( int i = 0; i != N; ++i ){//döngü
		test.insert( rand() );//insert bloðuna gider
	}
	int a;//degisken tanýmlama
	test.display();//agacý görüntüleme
	while( cin >> a ){//a degeri girilirdiyse
		test.erase( a );//a degerini agactan silip
		test.display();//agacý yeniden gösterir
	}
	return 0;
}
