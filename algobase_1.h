#ifndef ALGOBASE_MUSTL
#define ALGOBASE_MUSTL

#include <cstring>
#include "iterator_1.h"
#include "util_1.h"

namespace mustl {
	#ifdef max  //���θôʵĺ궨��  �ڱ������ռ��� max������㶨��
    #pragma message("#undefing marco max")
    #undef max
    #endif // max

    #ifdef min
    #pragma message("#undefing marco min")
    #undef min
    #endif // min

    /*****************************************************************************************/
    // max
    // ȡ�����еĽϴ�ֵ���������ʱ��֤���ص�һ������
    /*****************************************************************************************/
    template <class T>
    const T& max(const T& lhs, const T& rhs)
    {
        return lhs < rhs ? rhs : lhs;
    }

    // ���ذ汾ʹ�ú������� comp ����Ƚϲ���
    template <class T, class Compare>
    const T& max(const T& lhs, const T& rhs, Compare comp)
    {
        return comp(lhs, rhs) ? rhs : lhs;
    }
    /*****************************************************************************************/
    // min 
    // ȡ�����еĽ�Сֵ���������ʱ��֤���ص�һ������
    /*****************************************************************************************/
    template <class T>
    const T& min(const T& lhs, const T& rhs)
    {
        return rhs < lhs ? rhs : lhs;
    }

    // ���ذ汾ʹ�ú������� comp ����Ƚϲ���
    template <class T, class Compare>
    const T& min(const T& lhs, const T& rhs, Compare comp)
    {
        return comp(rhs, lhs) ? rhs : lhs;
    }

    /*****************************************************************************************/
    // iter_swap
    // ��������������ָ����Ե�
    /*****************************************************************************************/
    template <class FIter1, class FIter2>
    void iter_swap(FIter1 lhs, FIter2 rhs)
    {
        mustl::swap(*lhs, *rhs);
    }
    /*****************************************************************************************/
    // copy
    // �� [first, last)�����ڵ�Ԫ�ؿ����� [result, result + (last - first))��
    /*****************************************************************************************/
    //���Ƕ�� copy--->������ /����ƽ�������ػ�  ������--->������ʵ�����/���������
    template <class InputIter, class OutputIter>
    OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result, mustl::input_iterator_tag) {

        for (; first != last; ++first, ++result) {
            *result = *first;
        }
        return result;
    }

    template <class RandomIter, class OutputIter>
    OutputIter unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result, mustl::random_access_iterator_tag) {

        for (auto n =last-first; n>0;--n, ++first, ++result) {
             *result = *first;
        }
        return result;
    }

    template <class Iter, class OutputIter>
    OutputIter unchecked_copy(Iter first, Iter last, OutputIter result) {

        return unchecked_copy_cat(first, last, result, iterator_category(first));
    }
    // Ϊ trivially_copy_assignable �����ṩ�ػ��汾 //ƽ����������---���ǽ����ƣ�������������������������Ҫ��Ķ���
    //char* �õײ����(memmove memcpy...)�ٶȿ�
    template <class T,class U> //std::memmove----��ֹ�ڴ��ص�
    typename mustl::enable_if<std::is_same<typename remove_const<T>::type,U>::value &&
                            std::is_trivially_copy_assignable<U>::value,U*>::type
    unchecked_copy(T* first,T* last,U* result) {
        const auto n = static_cast<size_t>(last - first); //nΪsize_t���� ��static_castǿת
        if (n != 0) {
            std::memmove(result, first, n*sizeof(U));//memmove�����ֽ�Ϊ��λ�ƶ��� ���Ҫ�ƶ�U�Ĵ�С*n
        }
        return result + n;
    }

    template<class Input ,class Output>
    Output copy(Input first, Input last, Output result) {
        return unchecked_copy(first, last, result);
    }


    /*****************************************************************************************/
    // copy_backward  ---ֻ��֧��˫�������/������ʵ�����
    // �� [first, last)�����ڵ�Ԫ�ؿ����� [result - (last - first), result)��
    /*****************************************************************************************/
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
        unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
            BidirectionalIter2 result, mustl::bidirectional_iterator_tag)
    {
        while (first != last)
            *--result = *--last;
        return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
        unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
            BidirectionalIter2 result, mustl::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n)
            *--result = *--last;
        return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
        unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
            BidirectionalIter2 result)
    {
        return unchecked_copy_backward_cat(first, last, result,
            iterator_category(first));
    }

    // Ϊ trivially_copy_assignable �����ṩ�ػ��汾
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
        std::is_trivially_copy_assignable<Up>::value,
        Up*>::type
        unchecked_copy_backward(Tp* first, Tp* last, Up* result)
    {
        const auto n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            result -= n;
            std::memmove(result, first, n * sizeof(Up));  //std::memmove���Դ����ڴ��ص�
        }
        return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
        copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return unchecked_copy_backward(first, last, result);
    }

    /*****************************************************************************************/
    // copy_if
    // ��[first, last)������һԪ���� unary_pred ��Ԫ�ؿ������� result Ϊ��ʼ��λ����
    /*****************************************************************************************/
    template <class InputIter, class OutputIter, class UnaryPredicate>
    OutputIter
        copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred)
    {
        for (; first != last; ++first)
        {
            if (unary_pred(*first))
                *result++ = *first;
        }
        return result;
    }

    /*****************************************************************************************/
    // copy_n 
    // �� [first, first + n)�����ϵ�Ԫ�ؿ����� [result, result + n)��
    // ����һ�� pair �ֱ�ָ�򿽱�������β��
    /*****************************************************************************************/
    template <class InputIter, class Size, class OutputIter>
    mustl::pair<InputIter, OutputIter>
        unchecked_copy_n(InputIter first, Size n, OutputIter result, mustl::input_iterator_tag)
    {
        for (; n > 0; --n, ++first, ++result)
        {
            *result = *first;
        }
        return mustl::pair<InputIter, OutputIter>(first, result);
    }

    template <class RandomIter, class Size, class OutputIter>
    mustl::pair<RandomIter, OutputIter>
        unchecked_copy_n(RandomIter first, Size n, OutputIter result,
            mustl::random_access_iterator_tag)
    {
        auto last = first + n;
        return mustl::pair<RandomIter, OutputIter>(last, mystl::copy(first, last, result));
    }

    template <class InputIter, class Size, class OutputIter>
    mustl::pair<InputIter, OutputIter>
        copy_n(InputIter first, Size n, OutputIter result)
    {
        return unchecked_copy_n(first, n, result, iterator_category(first));
    }

    /*****************************************************************************************/
    // move
    // �� [first, last)�����ڵ�Ԫ���ƶ��� [result, result + (last - first))��
    /*****************************************************************************************/
    // input_iterator_tag �汾
    template <class InputIter, class OutputIter>
    OutputIter
        unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
            mustl::input_iterator_tag)
    {
        for (; first != last; ++first, ++result)
        {
            *result = mystl::move(*first);
        }
        return result;
    }

    // ramdom_access_iterator_tag �汾
    template <class RandomIter, class OutputIter>
    OutputIter
        unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result,
            mustl::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n, ++first, ++result)
        {
            *result = mystl::move(*first);
        }
        return result;
    }

    template <class InputIter, class OutputIter>
    OutputIter
        unchecked_move(InputIter first, InputIter last, OutputIter result)
    {
        return unchecked_move_cat(first, last, result, iterator_category(first));
    }

    // Ϊ trivially_copy_assignable �����ṩ�ػ��汾
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
        std::is_trivially_move_assignable<Up>::value,
        Up*>::type
        unchecked_move(Tp* first, Tp* last, Up* result)
    {
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0)
            std::memmove(result, first, n * sizeof(Up));
        return result + n;
    }

    template <class InputIter, class OutputIter>
    OutputIter move(InputIter first, InputIter last, OutputIter result)
    {
        return unchecked_move(first, last, result);
    }

    /*****************************************************************************************/
    // move_backward
    // �� [first, last)�����ڵ�Ԫ���ƶ��� [result - (last - first), result)��
    /*****************************************************************************************/
    // bidirectional_iterator_tag �汾
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
        unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
            BidirectionalIter2 result, mustl::bidirectional_iterator_tag)
    {
        while (first != last)
            *--result = mystl::move(*--last);
        return result;
    }

    // random_access_iterator_tag �汾
    template <class RandomIter1, class RandomIter2>
    RandomIter2
        unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
            RandomIter2 result, mustl::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n)
            *--result = mystl::move(*--last);
        return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
        unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last,
            BidirectionalIter2 result)
    {
        return unchecked_move_backward_cat(first, last, result,
            iterator_category(first));
    }

    // Ϊ trivially_copy_assignable �����ṩ�ػ��汾
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
        std::is_trivially_move_assignable<Up>::value,
        Up*>::type
        unchecked_move_backward(Tp* first, Tp* last, Up* result)
    {
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            result -= n;
            std::memmove(result, first, n * sizeof(Up));
        }
        return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
        move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return unchecked_move_backward(first, last, result);
    }

    //�Ƚ�����1��first��last�������Ƿ������2���
    template<class InputIter1,class InputIter2>
    bool equal(InputIter1 first, InputIter2 last, InputIter2 result) {
        while (first != last) {
            if (*first != *result)
                return false;
            ++result;
            ++first;
        }
        return true;
    }


    // ���ذ汾ʹ�ú������� comp ����Ƚϲ���
    template <class InputIter1, class InputIter2, class Compared>
    bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared comp)
    {
        for (; first1 != last1; ++first1, ++first2)
        {
            if (!comp(*first1, *first2))
                return false;
        }
        return true;
    }

    //fill_n ��firstλ�����n���� ����result
    template <class OutputIter, class Size, class T>
    OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
    {
        for (; n > 0; --n, ++first)
        {
            *first = value;
        }
        return first;
    }
    // Ϊ one-byte �����ṩ�ػ��汾 (bool���飿)�����Ҫis_same bool ���� is_intergal
    //memset һ�����ڴ������ ��Ŀ�ģ����ֵ��������
    template <class Tp, class Size, class Up>
    typename std::enable_if<
        std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
        !std::is_same<Tp, bool>::value &&
        std::is_integral<Up>::value && sizeof(Up) == 1,
        Tp*>::type
        unchecked_fill_n(Tp* first, Size n, Up value)
    {
        if (n > 0)
        {
            std::memset(first, (unsigned char)value, (size_t)(n));
        }
        return first + n;
    }

    template <class OutputIter, class Size, class T>
    OutputIter fill_n(OutputIter first, Size n, const T& value)
    {
        return unchecked_fill_n(first, n, value);
    }

    // fill
    // Ϊ [first, last)�����ڵ�����Ԫ�������ֵ
    /*****************************************************************************************/
    template <class ForwardIter, class T>
    void fill_cat(ForwardIter first, ForwardIter last, const T& value,
        mustl::forward_iterator_tag)
    {
        for (; first != last; ++first)
        {
            *first = value;
        }
    }

    template <class RandomIter, class T>
    void fill_cat(RandomIter first, RandomIter last, const T& value,
        mustl::random_access_iterator_tag)
    {
        fill_n(first, last - first, value);
    }

    template <class ForwardIter, class T>
    void fill(ForwardIter first, ForwardIter last, const T& value)
    {
        fill_cat(first, last, value, iterator_category(first));
    }


    //lex...���ֵ���������

    template <class InputIter1, class InputIter2>
    //����ȽϵĹ����У����first1<first2 return true ��֮return false
    //�����һ���ȵ���ĩβ����ô����first1����first2�ȵ���
    //�����frist1�ȵ� ����false
    bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (*first1 < *first2)
                return true;
            if (*first2 < *first1)
                return false;
        }
        return first1 == last1 && first2 != last2;
    }

    //���unsigned char*�ػ�          //��memcmp�Ƚ���ͬ���ȵ��ȣ�Ȼ���ٿ�ʣ�µĲ��ֳ���
    bool lexicographical_compare(const unsigned char* first1,
        const unsigned char* last1,
        const unsigned char* first2,
        const unsigned char* last2)
    {
        const auto len1 = last1 - first1;
        const auto len2 = last2 - first2;
        // �ȱȽ���ͬ���ȵĲ���
        const auto result = std::memcmp(first1, first2, mustl::min(len1, len2));
        // ����ȣ����Ƚϳ��ıȽϴ�
        return result != 0 ? result < 0 : len1 < len2;
    }

    //mismatch �ҵ���һ����ƥ���Ԫ��---����һ��pair
    template <class InputIter1, class InputIter2>
    mustl::pair<InputIter1, InputIter2>
        mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
    {
        while (first1 != last1 && *first1 == *first2)
        {
            ++first1;
            ++first2;
        }
        return mystl::pair<InputIter1, InputIter2>(first1, first2);
    }

    // ���ذ汾ʹ�ú������� comp ����Ƚϲ���
    template <class InputIter1, class InputIter2, class Compred>
    mustl::pair<InputIter1, InputIter2>
        mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compred comp)
    {
        while (first1 != last1 && comp(*first1, *first2))
        {
            ++first1;
            ++first2;
        }
        return mystl::pair<InputIter1, InputIter2>(first1, first2);
    }
}
#endif