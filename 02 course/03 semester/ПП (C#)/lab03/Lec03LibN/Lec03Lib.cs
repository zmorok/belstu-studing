using Lec03LibN.FactoryLevels;

namespace Lec03LibN
{
    public static class Lec03Lib
    {
        public static IFactory getL1() => new FactoryLevel1();

        public static IFactory getL2(float a) => new FactoryLevel2(a);

        public static IFactory getL3(float a, float b) => new FactoryLevel3(a, b);
    }
}
