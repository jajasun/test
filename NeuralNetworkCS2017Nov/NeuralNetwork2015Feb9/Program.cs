using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NN;
namespace NeuralNetwork2015
{
    
    class Program
    {
        static void Main(string[] args)
        {
             Brain brain = new Brain();
             brain.Initialize();
             brain.Training();
             Console.ReadLine();
        }
    }
}
