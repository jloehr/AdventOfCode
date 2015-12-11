using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Day4
{
    class Program
    {
        static void Main(string[] args)
        {
            String Input = "ckczppom";
            MD5 Hash = MD5.Create();
            UInt64 Counter = 0;
            byte[] Result;

            UInt64 FiveZeros = 0;
            UInt64 SixZeros = 0;

            do
            {
                Counter++;
                String HashingString = Input + Counter;
                Result = Hash.ComputeHash(Encoding.ASCII.GetBytes(HashingString));

                if(FiveZeros == 0)
                {
                    if(HasEnoughLeadingZeros(Result, 5))
                    {
                        FiveZeros = Counter;
                    }
                }

                if (SixZeros == 0)
                {
                    if (HasEnoughLeadingZeros(Result, 6))
                    {
                        SixZeros = Counter;
                    }
                }

            } while ((FiveZeros == 0) || (SixZeros == 0));

            System.Console.WriteLine("Five: {0}", FiveZeros);
            System.Console.WriteLine("Six: {0}", SixZeros);
            Console.Write("Press any key to continue . . .");
            Console.ReadKey(true);
        }

        static bool HasEnoughLeadingZeros(byte[] Hash, uint RequiredZeros)
        {
            uint LeadingZeros = 0;

            for (int i = 0; i < Hash.Length; i++)
            {
                if (Hash[i] != 0x00)
                {
                    if ((Hash[i] <= 0x0F))
                    {
                        LeadingZeros++;
                        return (LeadingZeros >= RequiredZeros);
                    }
                    else
                    {
                        return false;
                    }
                }

                LeadingZeros += 2;
                if (LeadingZeros >= RequiredZeros)
                {
                    return true;
                }
            }

            return false;
        }
    }
}
