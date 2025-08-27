namespace Lec03LibN
{
    public class Employee
    {
        public IBonus bonus { get; set; }

        public Employee(IBonus bonus)
        {
            this.bonus = bonus;
        }

        public float calcBonus(float wH)
        {
            return bonus.calc(wH);
        }
    }
}
