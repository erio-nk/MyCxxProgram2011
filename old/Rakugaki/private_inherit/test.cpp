// 基底クラス
class Base
{
public:
	int _publicData;
	void PublicFunc() {}

protected:
	int _protectedData;
	void ProtectedFunc() {}

private:
	int _privateData;
	void PrivateFunc() {}
};

// private継承したクラス
class Derived : private Base
{
	// クラス内から基底クラスのメンバへアクセス
	void Do()
	{
		_publicData = 0;		// OK ; publicにはアクセス可。
		PublicFunc();			// OK

		_protectedData = 0;		// OK ; 同上。
		ProtectedFunc();		// OK

		_privateData = 0;		// NG ; 当然の如くprivateにはアクセス不可。
		PrivateFunc();			// NG
	}
};

// main
int main()
{
	Derived obj;

	// 基底クラスのポインタに変換
	//Base* pBase = &obj;		// NG ; 基底クラスに変換出来ない！

	// クラス外から基底クラスのメンバへアクセス
	//obj._publicData = 0;		// NG ; publicメンバにもアクセス出来ない！
	//obj.PublicFunc();			// NG

//	obj._protectedData = 0;		// NG ; protectedは当然アクセス不可。
//	obj.ProtectedFunc();		// NG

//	obj._privateData = 0;		// NG ; privateも当然アクセス不可。
//	obj.PrivateFunc();			// NG

	return 0;
}
