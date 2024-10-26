/*
 * 前端会点C++怎么了 ——YuWu 2024.10.26
 */


#ifndef 遗忘之物_REDIRECTPOINTER_H
#define 遗忘之物_REDIRECTPOINTER_H


/**
 * 重定向指针函数
 *
 * 此函数用于将一个指针变量的指向更改为一个新的地址。
 * 它通过直接操作内存来实现，因此需要谨慎使用。
 *
 * @tparam T 指针指向的类型
 * @param originalPtrAddress 原始指针变量的地址
 * @param newPtrAddress 新的地址
 */
template<typename T>
void redirectPointer(uintptr_t originalPtrAddress, uintptr_t newPtrAddress) {

    LOGI("原始指针：%p 更新后: %p", originalPtrAddress, newPtrAddress);

    // 将原始指针地址转换为指向指针的指针（即双指针）
    // 这样可以修改原始指针变量所指向的地址
    T** originalPtr = reinterpret_cast<T**>(originalPtrAddress);

    // 将新地址转换为 T* 类型的指针
    // 然后将其赋值给原始指针变量所指向的地址
    // 从而实现指针重定向
    *originalPtr = reinterpret_cast<T*>(newPtrAddress);
}

/**
 * 注意：
 * 1. originalPtrAddress 和 newPtrAddress 必须是有效的地址。
 * 2. 使用此函数时需要确保转换的安全性，错误的转换可能导致程序崩溃或数据损坏。
 * 3. 此函数直接操作指针，可能会导致未定义行为。
 * 4. 此函数是一个低级别的操作，通常只在特定的编程场景中使用。
 */

#endif //遗忘之物_REDIRECTPOINTER_H
