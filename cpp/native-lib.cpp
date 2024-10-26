/*
 * 前端会点C++怎么了 ——YuWu 2024.10.26
 */

#include <jni.h>
#include <android/log.h>
#include <string>
#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>
#include <memory>
#include "EFMod/EFMod.hpp"

//日志宏定义
#define LOG_TAG "遗忘之物"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__))

#include "redirectPointer.h"
#include "ApiPtr.h"



/**
 * New_ItemIDSets 函数用于初始化和重置游戏中的物品ID集合设置。
 * 这个函数创建一个新的布尔数组，并将其用于更新游戏中的物品设置。
 */
void New_ItemIDSets() {
    // 定义数组的大小，这个值可能与游戏中物品的数量有关
    size_t size = 5456;

    // 使用 new 关键字动态分配一个布尔数组，大小为 size
    bool* trueArray = new bool[size];

    // 遍历数组，将所有元素初始化为 false
    // 这可能表示默认情况下，所有物品ID都不被标记为“已弃用”或“不应该在库存中”
    for (size_t i = 0; i < size; ++i) {
        trueArray[i] = false;
    }

    // 使用 BNM 库的函数创建一个新的 Mono 类型的布尔数组
    // 这个数组将被用于替换游戏中原有的设置
    BNM::Structures::Mono::Array<bool>* myArray = BNM::Structures::Mono::Array<bool>::Create(trueArray, size);

    // 使用 redirectPointer 函数重定向游戏中的 Deprecated 指针到新的 myArray
    // Deprecated 可能是一个指向游戏中“已弃用”物品ID集合的指针
    redirectPointer<void*>(reinterpret_cast<uintptr_t>(Deprecated->GetPointer()), reinterpret_cast<uintptr_t>(myArray));
    // 打印日志信息，表示“已解除禁装限制！”
    LOGI("已解除禁装限制！");

    // 同样地，重定向 ItemsThatShouldNotBeInInventory 指针到新的 myArray
    // ItemsThatShouldNotBeInInventory 可能是一个指向游戏中“不应该在库存中”的物品ID集合的指针
    redirectPointer<void*>(reinterpret_cast<uintptr_t>(ItemsThatShouldNotBeInInventory->GetPointer()), reinterpret_cast<uintptr_t>(myArray));
    // 打印日志信息，表示“已解除“不应该存在库存”限制！”
    LOGI("已解除“不应该存在库存”限制！");
}





class ForgottenItem: public EFMod {

public:

    //获取Mod标识
    const char* GetIdentifier() const override {
        return "ForgottenItem-YuWu"; //返回MOD标识
    }

    //初始化函数
    bool Initialize() override {
        LOGI("已尝试加载[遗忘之物]");
        return isInitialize;
    }

    //注册扩展
    void RegisterHooks() override {
        /**
         * @RegisterExtension 注册扩展函数的API
         * 注册扩展函数，这里为Assembly-CSharp.dll.Terraria.ID.ItemID.Sets..cctor的扩展
         * @New_ItemIDSets 函数为原函数的扩展函数，会被原函数调用
         */
        EFMod->RegisterExtension("Assembly-CSharp.dll.Terraria.ID.ItemID.Sets..cctor", reinterpret_cast<long>(New_ItemIDSets));
    }

    //注册API
    void RegisterAPIs() override {
        /**
         * 注册一个API，名为 "Terraria.ID.ItemID.Sets.Deprecated"。
         * 这个API指向一个布尔数组，表示哪些物品ID是被弃用的。
         * 通过注册这个API，其他模组可以检查或修改弃用物品的列表。
         */
        EFMod->RegisterAPI("Terraria.ID.ItemID.Sets.Deprecated", reinterpret_cast<uintptr_t>(&Deprecated));

        /**
         * 注册一个API，名为 "Terraria.ID.ItemID.Sets.ItemsThatShouldNotBeInInventory"。
         * 这个API指向一个布尔数组，表示哪些物品ID不应该出现在玩家的库存中。
         * 通过注册这个API，其他模组可以检查或修改不允许在库存中的物品列表。
         */
        EFMod->RegisterAPI("Terraria.ID.ItemID.Sets.ItemsThatShouldNotBeInInventory", reinterpret_cast<uintptr_t>(&ItemsThatShouldNotBeInInventory));

        /**
         * 注册一个API，名为 "Terraria.ID.ItemID.Sets.ShimmerTransformToItem"。
         * 这个API指向一个整型数组，表示哪些物品ID可以被转换为其他物品。
         * 通过注册这个API，其他模组可以检查或修改物品转换的规则。
         */
        EFMod->RegisterAPI("Terraria.ID.ItemID.Sets.ShimmerTransformToItem", reinterpret_cast<uintptr_t>(&ShimmerTransformToItem));
    }

    //加载EFMod基本API
    void LoadEFMod(EFModLoaderAPI* Mod) override {
        /**
         * @Mod 加载器会提供此指针，不需要在意
         * @EFMod 保存加载器提供的指针
         */
        EFMod = Mod; //保存指针

        if (EFMod == nullptr /* 判断API指针是否有效 */){
            LOGE("无法获取API指针，加载失败！"); //打印失败日志
            isInitialize = false; //更改加载状态
            return; //结束函数
        }
        LOGI("已获取API指针，加载成功！"); //打印成功日志
    }

private:
    /**
     * @EFMod 保存加载器提供的API指针
     * @isInitialize 加载状态
     */
    EFModLoaderAPI* EFMod = nullptr;
    bool isInitialize = true;
};


/**
 * @GetModInstance 获取Mod实例，此函数是必须的！
 */
extern "C" EFMod* GetModInstance() {
    static ForgottenItem ForgottenItem;
    return &ForgottenItem;
}