#ifndef TYPE_TRAITS_MUSTL
#define TYPE_TRAITS_MUSTL

// ���ͷ�ļ�������ȡ������Ϣ
//������ȡ
// use standard header for type_traits
#include <type_traits>
namespace mustl
{   

	//���������ʵ��std::true_type std::false_type 
	template <class T, T v>                                  //��ģ�� һ������ΪT�Ĳ��� �Լ�����ΪT�ı���v
	struct integral_constant
	{
		static constexpr T value = v;                        //value��һ��boolֵ Ϊ���type_traits�����ĵײ㲿��
	};	                                                     //���徲̬��Ա����(����) 1.���轨�����󼴿ɵ��øó�Ա---- mustl::integral_constant::value;
									                         //                       2.��̬����������Ϊȫ�� ���ж����øó�Ա
		                                                    //constexpr-------����ֵ���ɱ����ڱ���������


	template <bool flag>                                   //����ػ�  T =bool  v=flag
	using bool_constant = integral_constant<bool, flag>;   //using֮�� ������������ -----bool_constant<true/false>  ֻ��ָ��flag��ֵ

	//���Ĳ��� std::true_type false_type                    ��������intergeal_constant�������������ػ��汾  ��һ�����ͣ�
	typedef bool_constant<true>  true_type;                 //�������bool_constant Ϊtrue---��true_type  ��֮��Ϊfalse_type
	typedef bool_constant<false>  false_type;               //�����ñ��ʽ���ߴ���bool���ͷ���ֵ�ĺ���������true��false �����Ϳ��Խ���������ȡ��

	/*****************************************************************************************/
	//��������true_type false_typeʵ�ֵ�һЩ���� 
	template <class T>                                     //������    
	struct is_int : mustl::false_type {};  
	
	template <>                                           //�ػ���     ֻ��is_int����"int"���Ͳż���̳���true_type���ػ��� ����ͽ��뷺����
	struct is_int<int> : mustl::true_type {};             //��ʱ���Ե��û����value�������鿴���:
	                                                      //��:   int a=1;
														  //      cout<<mustl::is_int<decltype(a)>::value<<endl; //true

	//***************************************************/
	// ����Ϊ���亯�� ʵ����һЩλ��type_traits.h�ڵĺ���
	//std::enable_if Ϊ��ģ�� ���ڱ���ʱ���������ж� �Լ�������������ʱ�������Ƶ�  
	template<bool,class T=void>                           //������ģ�����  bool������T   ��boolΪtrueʱ�����ػ��� ������ȡT����  
	struct enable_if{};  //����                           //Ҳ�����ñ��ʽ���ߴ���bool���ͷ���ֵ�ĺ���

	template<class T>  //�ػ�                             //��:  typename mustl::enable_if<(1<2),int>::type a =1;
	                                                             
	struct enable_if<true, T> {
		typedef  T type;
	};
	//********************************************
	//conditional �ṹ��ģ�� ������Ŀ����� <bool,t1,t2>      ԭ��ͬ��  true--->T1��֧  false--->T2��֧
	template<bool,class T1,class T2>//����                    //����Ƕ������
	                                                          //mustl::conditional<(1<2),mustl::conditional<true,int,int>::type,void>::type
	struct conditional {
		typedef T1 type;
	};

	template<class T1,class T2>    //�ػ�
	struct conditional<false,T1,T2>{
		typedef T2 type;
	}; 
	
	
	//******************************************** ������ȡ
	//remove_all_extents  ȡ�������ڲ�Ԫ�ص�����  ����ֱ�Ӷ������ȥ Ҳ����decltype(a)��ȡ���ʽ�����ٶ���ȥ
	//Ҫô�� int a,int a[],int a[4]
	template <class T>                  
	struct remove_all_extents {
		typedef T value;
	};

	template<class T>                           //�ػ�1  ��� remove_all_extents<a[]> ���ڲ�����ȥ[]�汾��  ��������ȡ��������Ԫ�ص�������
	struct remove_all_extents<T[]> {      
		typedef typename remove_all_extents<T>::value value;
	};

	template<class T,size_t N>
	struct remove_all_extents<T[N]> {      //�ػ�2      ��� remove_all_extents<a[3]> ���ڲ�����ȥ[]�汾��
		typedef typename remove_all_extents<T>::value value;
	};

	//*****************************************
	//std::decay ʵ���˻�(ȥ��const�������������η�������->ָ�룬����->ָ��)
	// std::decay<decltype(a)>::type
	template <class T1>                         //ȥconst ����
	struct remove_const {
		using type = T1;
	};

	template <class T1>
	struct remove_const<const T1> {          //�ػ�
		using type = T1;
	};
	//****************************************
	template <class T1>                        //ȥ���� ����
	struct remove_reference {
		using type = T1;
	};

	template <class T1>
	struct remove_reference<T1&> {           //�ػ�
		using type = T1;
	};

	template <class T1>
	struct remove_reference<T1&&> {           //�ػ�
		using type = T1;
	};
	//std::decay
	template<class T1> //������---ȥ���á�const  ������ȥ���ú�ȥconst
	struct decay:remove_const<remove_reference<T1>>{};

	template<class T1,size_t size>  //�ػ�     ����--->ָ��
	struct decay<T1[size]> {
		using type = T1*;
	};
	                               //�ػ�     ����->����ָ��  T1---��������ֵ���� ...argsΪ�ɱ�� ����func(int a,int b, string c)����Ϊ��ĳ�����ʱ����
	                               // func(args)...������ʽ���  
	template <class T1,class...args> //  auto p =decay<func(1,2,"ss")>::type    ------p��func�ĺ���ָ��
	struct decay<T1(args...)> {
		using type = T1 * (args...);
	};
}

#endif


