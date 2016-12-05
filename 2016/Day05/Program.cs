using System;
using System.Security.Cryptography;
using System.Text;

namespace Day05
{
    class Program
    {
        static void Main(string[] args)
        {
            MD5 Hash = MD5.Create();
            string Input = "ffykfhsq";
            string Password = "";
            UInt64 Index = 0;

            while(Password.Length < 8)
            {
                string HashString = Input + Index;
                byte[] Result = Hash.ComputeHash(Encoding.ASCII.GetBytes(HashString));

                Password += GetPassordCharacter(Result);

                Index++;
            }

            Console.WriteLine(Password);
            Console.Write("Press any key to continue . . .");
            Console.ReadKey(true);
        }

        static string GetPassordCharacter(byte[] Hash)
        {
            if(HasEnoughLeadingZeros(Hash, 5))
            {
                return (Hash[2] & 0x0F).ToString("x");
            }
            else
            {
                return String.Empty;
            }
        }

        static bool HasEnoughLeadingZeros(byte[] Hash, uint RequiredZeros)
        {
            for(int i = 0; i < Hash.Length; i++)
            {
                if(Hash[i] != 0x00)
                {
                    if(Hash[i] <= 0x0F)
                    {
                        return (((i * 2) + 1) >= RequiredZeros);
                    }
                    else
                    {
                        return false;
                    }
                }

                if(((i + 1) * 2) >= RequiredZeros)
                {
                    return true;
                }
            }

            return false;
        }
    }
}
