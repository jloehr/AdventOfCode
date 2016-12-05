using System;
using System.Security.Cryptography;
using System.Text;

namespace Day05
{
    class Program
    {
        struct Cursor
        {
            public int Top;
            public int Left;
        }

        static void Main(string[] args)
        {
            MD5 Hash = MD5.Create();
            string Input = "ffykfhsq";

            int PartOneLength = 0;
            string PartTwo = "00000000";
            bool[] CharachterLocked = new bool[8];
            int PartTwoRemainingBlanks = 8;
            UInt64 Index = 0;

            Cursor PartOneCursor;
            Cursor PartTwoCursor;

            Console.Write("Part One: ");
            PartOneCursor.Top = Console.CursorTop;
            PartOneCursor.Left = Console.CursorLeft;
            Console.WriteLine();
            Console.Write("Part Two: ");
            PartTwoCursor.Top = Console.CursorTop;
            PartTwoCursor.Left = Console.CursorLeft;

            while (PartTwoRemainingBlanks > 0)
            {
                string HashString = Input + Index;
                byte[] Result = Hash.ComputeHash(Encoding.ASCII.GetBytes(HashString));

                bool ValidHash = HasEnoughLeadingZeros(Result, 5);
                bool DoFancyOutput = (Index % 100 == 0);

                if (ValidHash || DoFancyOutput)
                {
                    int Sixth = (Result[2] & 0x0F);
                    int Seventh = ((Result[3] & 0xF0) >> 4);
                    string Character = Seventh.ToString("x");              

                    if (PartOneLength < 8)
                    {
                        Console.SetCursorPosition(PartOneCursor.Left, PartOneCursor.Top);
                        Console.Write(Sixth.ToString("x"));

                        if(ValidHash)
                        {
                            PartOneCursor.Left++;
                            PartOneLength++;
                        }
                    }

                    if ((Sixth < PartTwo.Length) && (!CharachterLocked[Sixth]))
                    {
                        PartTwo = PartTwo.Substring(0, Sixth) + Character + PartTwo.Substring(Sixth + 1, 7 - Sixth);

                        if (ValidHash)
                        {
                            CharachterLocked[Sixth] = true;
                            PartTwoRemainingBlanks--;
                        }

                        Console.SetCursorPosition(PartTwoCursor.Left, PartTwoCursor.Top);
                        Console.Write(PartTwo);
                    }
                }               

                Index++;
            }

            Console.WriteLine();
            Console.Write("Press any key to continue . . .");
            Console.ReadKey(true);
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
