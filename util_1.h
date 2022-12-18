#ifndef UTIL_1
#define UTIL_1
#include <cstddef>
#include "type_traits_1.h"
namespace mustl {
	//std::move ��ֵת��ֵ
	//ʹ��static_castǿת 
	template <class T>
	typename std::remove_reference<T>::type && move(T&& arg) {  //�����������Ƶ�--->�������� ���۽���������ֵ��ֵ����ȥ�������� Ȼ��ǿתΪ��ֵ���� 
		return static_cast<typename std::remove_reference<T>::type &&>(arg);
	}
	//std::forward ����ת�� ��֤�����ڴ��ݹ���������ʼ�ձ��ֲ���
	//��Ϊ T T& T&&
	template<class T>
	T&& forward(typename std::remove_reference<T>::type& arg) {
		return static_cast<T&&>(arg);
		//����������ֵ �Ƶ��� T =x& չ���õ� x& && ----�������۵�֮�󷵻���ֵ
	}

	template <class T>
	T&& forward(typename std::remove_reference<T>::type&& arg) {
		//�����ʱʶ�����Ϊ��ֵ �ͱ���
		// target:������ֵ   
		static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
		return static_cast<T&&>(arg);
		//����������ֵ �Ƶ��� T =x&& չ���õ� x&& && ----�������۵�֮�󷵻���ֵ
	}
	//*****************************************
	//swap  ʹ��moveЧ�ʸ��� ֱ�ӽ�������Ȩ��ת��
	template<class T1>                 //����һ������
	void swap(T1& lhs, T1& rhs) {
		T1 temp = mustl::move(lhs);
		lhs = mustl::move(rhs);
		rhs = mustl::move(temp);
	}

	template<class T1, size_t N>
	void swap(T1(&a)[N], T1(&b)[N]) {    //��������
		if (&a != &b) { //���A B ��ַ�Ƿ���ͬ �����ͬ��˵��A=B ���轻��
			T1* firstA = a;
			T1* lastA = a + N;
			T1* firstB = b;
			for (; firstA != lastA; ++firstA, ++firstB) {
				mustl::swap(*firstA, *firstB);
			}
		}
	}
	//*********************************************
	//**********************************************
	//std::pair 
	template<class T1,class T2>
	struct pair {
		typedef T1 first_type;
		typedef T2 second_type;

		first_type first;
		second_type second;

		//���ֹ��캯��  ��constexpr ʹ���ڱ�����ִ��
		// Other1 Other2����Ĭ��ֵ 
		//1)Ĭ�Ϲ��캯��    T1 T2�Ƿ���Ĭ�Ϲ���?      
		template<class Other1 = T1, class Other2 = T2, 
			typename mustl::enable_if<
			std::is_default_constructible<Other1>::value&&
			std::is_default_constructible<Other2>::value, void>::type>
			constexpr pair() :first(), second() {};

		//2)��ʽ���캯��   
		//�����ܹ����п������죿�ܷ����const T1&->T1,const T2&->T2����ʽת����
		template<class Other1 = T1, class Other2 = T2, 
			typename mustl::enable_if<
			std::is_copy_constructible<Other1>::value&&
			std::is_copy_constructible<Other2>::value&&
			std::is_convertible<const Other1&,T1>::value &&
			std::is_convertible<const Other2&, T2>::value, int>::type=0>
			constexpr pair(const T1&a,const T2&b) :first(a), second(b) {};

		//3)��ʽ���캯��    
		//�����ܹ����п������죿 const T1&,const T2&�Ƿ�����һ�������޷���ʽת����
		template<class Other1 = T1, class Other2 = T2, 
			typename mustl::enable_if<
			std::is_copy_constructible<Other1>::value&&
			std::is_copy_constructible<Other2>::value&&
			(!std::is_convertible<const Other1&, T1>::value||
			!std::is_convertible<const Other2&, T2>::value), int>::type = 0>
			explicit constexpr pair(const T1& a, const T2& b) :first(a), second(b) {};//��explicit��ʾ�޷���ʽ����

		pair(const pair& rhs) = default;
		pair(pair&& rhs) = default; //���������Լ��ƶ��������Ĭ��


		////////////////���濪ʼ�е������ �ҵ�ע��Ҳ��һ����
		/////֮ǰ����T1 T2��������ʼ��pair  4)-5)��ʹ��������������ʼ��pair�����T1 T2������Other1 Other2����,�ҿ�����ʽת�� ��Ҳ����ʹ��Other1 Other2����ʼ��pair
		///6)-9)����pair����ʼ��pair

		//is_constructible<T1,...args> T1�Ƿ���Ա�args...������  <int,float,double>--->int(float,double)?
		// ����ʵ��T1(Other1) ��T2(Other2)�����Ĺ�����?
		//4)�������͵���ʽ���캯��   �������Ϳ�����ʽת��ΪT1 T2�� ��ʱΪ��֤������ȷ���� ʹ���������ú�����ת��
		template<class Other1, class Other2, 
			typename mustl::enable_if<
			std::is_constructible<T1,Other1>::value&&
			std::is_constructible<T2,Other2>::value&&
			std::is_convertible<Other1&&, T1>::value&&
			std::is_convertible<Other2&&, T2>::value, int>::type = 0>
			constexpr pair(Other1&& a, Other2&& b) :first(mustl::forward<Other1>(a)), second(mustl::forward<Other1>(b)) {};

		//5)�������͵���ʾ���캯��   -->pair<int,int>a =pair<int,int>(1,2)   
         //�����ܹ����죿 �Ƿ�����һ�������޷���ʽת���� ��ʱΪ��֤������ȷ���� ʹ���������ú�����ת��
		template<class Other1, class Other2, 
			typename mustl::enable_if<
			std::is_constructible<T1,Other1>::value&&
			std::is_constructible<T2,Other2>::value &&
			(!std::is_convertible<Other1, T1>::value ||
				!std::is_convertible<Other2, T2>::value), int>::type = 0>
			explicit constexpr pair(Other1&& a, Other2&& b) :first(mustl::forward<Other1>(a)), second(mustl::forward<Other1>(b)) {};

		//6ʹ��const pair������ʽ��ʼ�� ʹ�� pair<other1,other2>��������һ��pair
		template<class Other1, class Other2, 
			typename mustl::enable_if<
			std::is_constructible<T1, const Other1&>::value&&
			std::is_constructible<T2, const Other2&>::value&&
			std::is_convertible<const Other1&, T1>::value&&
			std::is_convertible<const Other2&, T2>::value, int>::type = 0>
			constexpr pair(const pair<Other1,Other2>&a) :first(a.first), second(a.second) {};

		//7)const pair��ʽ��ʼ�� ʹ�� pair<other1,other2>��������һ��pair
		template<class Other1, class Other2, 
			typename mustl::enable_if<
			std::is_constructible<T1, const Other1&>::value&&
			std::is_constructible<T2, const Other2&>::value&&
			(!std::is_convertible<const Other1&, T1>::value||
			!std::is_convertible<const Other2&, T2>::value), int>::type = 0>
			explicit constexpr pair(const pair<Other1, Other2>& a) :first(a.first), second(a.second) {};

		//8)��const pair����ʽ��ʼ��       ��������+����ת��׼û�� && +forward
		template<class U1, class U2, typename mustl::enable_if<
			std::is_constructible<T1, U1>::value&&
			std::is_constructible<T2, U2>::value&&
			std::is_convertible<U1, T1>::value&&
			std::is_convertible<U2, T2>::value, int>::type = 0>
			constexpr pair(pair<U1, U2>&& a):first(mustl::forward<U1>(a.first)), second(mustl::forward<U2>(a.second)){};

		//8)��const pair����ʽ��ʼ��       
		template<class U1, class U2, typename mustl::enable_if<
			std::is_constructible<T1, U1>::value&&
			std::is_constructible<T2, U2>::value&&
			(!std::is_convertible<U1, T1>::value||
			!std::is_convertible<U2, T2>::value), int>::type = 0>
			explicit constexpr pair(pair<U1, U2>&&a) :first(mustl::forward<U1>(a.first)), second(mustl::forward<U2>(a.second)) {};


		//������ֵ�����
		//1)const
		pair& operator=(const pair & a){
			//����Ը�ֵ
			if (this != &a) {
				this->first = a.first;
				this->second = a.second;
			}
			return *this;
		}
		//2)�ƶ���ֵ�����
		pair& operator=(pair&& a) {
			//����Ը�ֵ
			if (this != &a) {
				this->first = mustl::move(a.first);
				this->second = mustl::move(a.second);
			}
			return *this;
		}
		// ��������pair�Ŀ��������
		template <class Other1, class Other2>
		pair& operator=(const pair<Other1, Other2>& other)
		{
			first = other.first;
			second = other.second;
			return *this;
		}

		// / ��������pair���ƶ������  ��������+����ת��
		template <class Other1, class Other2>
		pair& operator=(pair<Other1, Other2>&& other)
		{
			first = mustl::forward<Other1>(other.first);
			second = mustl::forward<Other2>(other.second);
			return *this;
		}

		~pair() {};

		void swap(pair& other) {
			//����Ը�ֵ
			if (this != &other) {
				mustl::swap(first, other.first);
				mustl::swap(second, other.second)
			}
		}
	}; 

	//���������
	// == <!= > <= >=
	template<class T1,class T2>
	bool operator==(const pair<T1,T2>& a, const pair<T1,T2>& b) { 
		return a.first == b.first && a.second == b.second;
	}

	template<class T1, class T2>
	bool operator<(const pair<T1, T2>& a, const pair<T1, T2>& b) {
		return (a.first<b.first) || (a.first == b.first && a.second<b.second);
	}

	template<class T1, class T2>
	bool operator!=(const pair<T1, T2>& a, const pair<T1, T2>& b) {
		return !(a==b);
	}
	template<class T1, class T2>
	bool operator>(const pair<T1, T2>& a, const pair<T1, T2>& b) {
		return b<a;
	}

	template<class T1, class T2>
	bool operator<=(const pair<T1, T2>& a, const pair<T1, T2>& b) {
		return !(b<a);
	}
	template<class T1, class T2>
	bool operator>=(const pair<T1, T2>& a, const pair<T1, T2>& b) {
		return !(a>b);
	}

	//����swap
	template<class T1,class T2>
	void swap(pair<T1, T2>& a, pair<T1, T2>& b) {
		a.swap(b);
	}
	//make_pair   ��������+����ת��    ������ת ���߳�������ֵ���ò�����ϵ�ʱ�� ���߲������ͷ�Χ��ʱ ����������������+����ת��
	template<class T1,class T2>
	pair<T1,T2> make_pair(T1 &&a, T2 &&b) {
		return pair<T1, T2>(mustl::forward<T1>(a), mustl::forward<T2>(b));
	}
}

#endif