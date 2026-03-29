#include <iostream>

namespace {
    struct NormalAttack {
        static constexpr int attack() { return 10; }
    };
    struct StrongAttack {
        static constexpr int attack() { return 20; }
    };
    struct MagicAttack {
        static constexpr int attack() { return 30; }
    };

    struct Alias {
        using P1 = NormalAttack;
        using P2 = StrongAttack;
        using P3 = MagicAttack;
    };

    template <typename T>
    struct SetT1 : virtual Alias {
        using P1 = T;
    };
    template <typename T>
    struct SetT2 : virtual Alias {
        using P2 = T;
    };
    template <typename T>
    struct SetT3 : virtual Alias {
        using P3 = T;
    };

    template <typename T, int N>
    struct Mid : T {};

    template <typename T1, typename T2, typename T3>
    struct SetBase : Mid<T1, 1>, Mid<T2, 2>, Mid<T3, 3> {};

    struct Args : virtual Alias {};

    template <typename T1 = Args, typename T2 = Args, typename T3 = Args>
    struct Character {
        using Policies = SetBase<T1, T2, T3>;
        constexpr int attack() { return Policies::P3::attack(); }
    };
}  // namespace

int main() {
    Character<> defaultCharacter;
    std::cout << "Default attack power:" << defaultCharacter.attack()
              << std::endl;
    Character<SetT1<MagicAttack>, SetT2<NormalAttack>, SetT3<StrongAttack>>
        customCharacter;
    std::cout << "Custom attack power: " << customCharacter.attack()
              << std::endl;
    return 0;
}