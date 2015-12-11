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

            do
            {
                Counter++;
                String HashingString = Input + Counter;
                Result = Hash.ComputeHash(Encoding.ASCII.GetBytes(HashingString));

            } while (!HasEnoughLeadingZeros(Result, 5));

            System.Console.WriteLine("Number: {0}", Counter);
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
