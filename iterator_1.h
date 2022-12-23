#ifndef ITERTOR_MUSTL
#define ITERTOR_MUSTL

#include "type_traits_1.h"
#include <cstddef>
namespace mustl {
   //�������ֵ�������
	//������ʵ�������˫���������ǰ������������������ 
	//���������
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag :public input_iterator_tag{};
	struct bidirectional_iterator_tag :public forward_iterator_tag{};
	struct random_access_iterator_tag : public bidirectional_iterator_tag{};

	//iteratorģ�� �ڲ�������Ԫ��    ���������͡��������ڲ�Ԫ�����͡�������֮��ľ��룬ָ��Ԫ�ص�ָ�룬ָ��Ԫ�ص�����
	template<class Category,class T,class Distance =ptrdiff_t,class Pointer =T*,class Reference =T&>
	struct iterator {
		typedef Category                             iterator_category;
		typedef T                                    value_type;
		typedef Pointer                              pointer;
		typedef Reference                            reference;
		typedef Distance                             difference_type; //ָʾ����������֮��ľ���
	};

	//�ж��Ƿ���ڵ�������
	template<class T>
	class has_iterator_cat {
	  private:
		  struct two {  //����һ��˽�� struct �����к������� ��СΪ����char�Ĵ�С
			  char a;
			  char b;
		   };

		  //SFINAE���� ƥ��ʧ�ܲ��Ǵ��� ��ȥѰ�Һ��ʵ�ģ��  �������UΪ���������� ��ô�ͷ���ֵ��СΪһ��char  else ����ֵΪtwo---����char��С
		  template<class U>
		  static two test(...) {//��̬��Ա�������������ڶ���
			  return two;
		  }
		  
		  template <class U>
		  static char test(typename U::iterator_category* p = nullptr) {
			                char c;
			                return c;
		  }
	  public:
		  static const bool value = sizeof(test<T>(0)) == sizeof(char); //��̬��Ա���þ�̬���� ��̬��Ա���Ը�ֵ ���������ڳ�ʼ��
	};


	///����      ���bool=true���ض�������
	template <class Iterator,bool>   //
	struct iterator_traits_impl {};

	template<class Iterator>
	struct iterator_traits_impl<Iterator, true> {
		//�����ض��� 
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
		typedef typename Iterator::difference_type   difference_type;
	};

	//_helper   ����֮ǰ��impl������ ʵ��Ƕ��
	//boolΪtrueʱ�ػ� Ȼ��̳�impl ��Iterator������ʽת��Ϊ��������(����iter�����iter)�е�һ��ʱ��impl��boolΪtrue ��ʱ���ø��ػ��汾
	template <class Iterator, bool>
	struct iterator_traits_helper {};

	template<class Iterator>
	struct iterator_traits_helper<Iterator, true> :public  
	iterator_traits_impl<Iterator,std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value ||
		                          std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
	{};

	//����֮ǰ����ȡ����������  
	template <class Iterator>    //���ж��Ƿ��е������� ��(true)--->iterator_traits_helper�ػ��汾
	struct iterator_traits:public iterator_traits_helper<Iterator,has_iterator_cat<Iterator>::value>{};

	//���ԭ��ָ�� �Լ�constָ����ػ��汾
// ���ԭ��ָ���ƫ�ػ��汾
	template <class T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag           iterator_category;
		typedef T                                    value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef ptrdiff_t                            difference_type;
	};

	template <class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag           iterator_category;
		typedef T                                    value_type;
		typedef const T* pointer;
		typedef const T& reference;
		typedef ptrdiff_t                            difference_type;
	};

	//�ж�T�Ƿ�ΪU������  1.���ж��Ƿ�Ϊ������ 2.���ж�T�ܷ���ʽת��ΪU������
    template<class T,class U, bool =has_iterator_cat<iterator_traits<T>>::value>  //��Ϊ֮ǰ������iterator_traits�ĸ��ְ汾
	struct has_iterator_cat_of: public bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value>{};

	//has_oterator_Cat_of<T,U,true/false>


	template <class T,class U>
	struct has_iterator_cat_of<T,U,false>:public false_type{};

	template<class Iter>
	struct is_input_itertor :public has_iterator_cat_of<Iter, input_iterator_tag> {};

	template <class Iter>
	struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

	template <class Iter>
	struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

	template <class Iter>
	struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

	template <class Iter>
	struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

	//is_iterator �ж��Ƿ�Ϊ������������������
	template<class Iter>
	struct is_iterator: public bool_constant<is_input_itertor<Iter>::value||is_output_iterator<Iter>::value>{};

	// ��ȡĳ���������� category
	template <class Iterator>
	typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator& )
	{
		typedef typename iterator_traits<Iterator>::iterator_category Category;  //�÷���һ�����Ͷ���
		return Category();
	}

	// ��ȡĳ���������� distance_type
	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type*   //����ֵΪָ������ ��static_castǿת
		distance_type(const Iterator& iter)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(iter);
	}

	// ��ȡĳ���������� value_type
	template <class Iterator>
	typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&iter)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(iter);
	}

	//distance  ---��������������������֮���㷨��һ��
	template <class InputIter>
	typename iterator_traits<InputIter>::different_type
	distance_range(InputIter first, InputIter end, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != end)
		{
			++first;
			++n;
		}
		return n;
	}

	template<class RandomIter>
	typename iterator_traits<RandomIter>::different_type
	distance_range(RandomIter first, RandomIter end, random_access_iterator_tag) {
		typename iterator_traits<RandomIter>::difference_type n = 0;
		return end-first;
	}

	template <class Iter>
	typename iterator_traits<Iter>::different_type
	distance(Iter first, Iter end) {
		return distance_range(first, end, iterator_category(first));
	}

	// ���º��������õ�����ǰ�� n ������

	// advance �� input_iterator_tag �İ汾
	template <class InputIterator, class Distance>
	void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
	{
		while (n--)
			++i;
	}

	// advance �� bidirectional_iterator_tag �İ汾
	template <class BidirectionalIterator, class Distance>
	void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
			while (n--)  ++i;
		else
			while (n++)  --i;
	}

	// advance �� random_access_iterator_tag �İ汾
	template <class RandomIter, class Distance>
	void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}

	template <class InputIterator, class Distance>
	void advance(InputIterator& i, Distance n)
	{
		advance_dispatch(i, n, iterator_category(i));
	}

	template<class Iterator>
	class reverse_iterator {
	private:
		Iterator current; //��ǰ����Ķ���
	public:
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type        value_type;
		typedef typename iterator_traits<Iterator>::difference_type   difference_type;
		typedef typename iterator_traits<Iterator>::pointer           pointer;
		typedef typename iterator_traits<Iterator>::reference         reference;

		typedef Iterator                                              iterator_type;
		typedef reverse_iterator<Iterator>                            self;

		// ���캯��
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type i) :current(i) {} //
		reverse_iterator(const self& rhs) :current(rhs.current) {}

		// ȡ����Ӧ�����������
		iterator_type base() const
		{
			return current;
		}
		//����*����� Ϊcurrent��ǰһ��
		reference operator*()const {
			auto temp = current;
			return *(--temp);
		}
		//���ء����������      ++i�������� i++������ʱ����
		pointer operator->()const {
			return &(operator*());
		}

		// ǰ��(++)��Ϊ����(--) ++i
		self& operator++()
		{
			--current;
			return *this;
		}

		self operator++(int) //i++
		{
			self temp = *this;
			--current;
			return temp;
		}
		// ����(--)��Ϊǰ��(++)
		self& operator--()
		{
			++current;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			++current;
			return tmp;
		}

		//+= -=   ��������
		self& operator+=(difference_type n) {
			current -= n;
			return *this;
		}
		self& operator-=(difference_type n) {
			current += n;
			return *this;
		}

		//+ - ������ʱ����
		self operator+(difference_type n)const {
			return self(current - n);
		}
		self operator-(difference_type n)const {
			return self(current + n);
		}

		//[] �������� ����������ʳ���� ��ʵ����
		reference operator[](difference_type n)const {
			return *((*this + n)); //-----��*((*(this+n)))
		}

		// ���رȽϲ�����  ��Ϊ���������������һ������Ϊthis ���ֻ�ܴ���һ������ ���Ҫʵ�����������Ļ� ��Ҫ����Ϊȫ�����������Ԫ����
		template <class Iterator>
		friend bool operator==(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return lhs.base() == rhs.base();
		}

		template <class Iterator>
		friend bool operator<(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return rhs.base() < lhs.base();
		}

		template <class Iterator>
		friend bool operator!=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return !(lhs == rhs);
		}

		template <class Iterator>
		friend bool operator>(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return rhs < lhs;
		}

		template <class Iterator>
		friend bool operator<=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return !(rhs < lhs);
		}

		template <class Iterator>
		friend bool operator>=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return !(lhs < rhs);
		}
	};
}
#endif
