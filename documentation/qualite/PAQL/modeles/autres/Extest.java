/**
 * @brief Résumé des responsabilités de la classe
 *
 * @version 1.0
 * @date 31/01/2013
 * @author Jonathan ILIAS-PILLET
 * @copyright BSD 2-clauses
 */
public class Example {

        private int value = 0;

        private static Example Singleton; // les attributs et méthodes de classe
                                          // commencent par une majuscule
              

        // les attributs et méthodes de classes sont placés en premier dans la classe

        // inutile de documenter les méthodes standards ou héritées
        public static void main (String[] args) {
                Singleton = new Example (10);
                
                try {
                        Singleton.show ();
                }
                catch (Exception e) {
                        System.out.println ("Exception !");
                }
        }
  
        /**
         * Crée un nouvel exemple avec la valeur initiale spécifiée
         *
         * @param initVal[in] valeur initiale choisie
         */
        public Example (int initVal) {
                value = initVal;
        }
        
        /**
         * Affiche la valeur de l'exemple
         *
         * @throws Exception déclenchée dans tous les cas
         */
        public void show () throws Exception {
                System.out.println ("Ma valeur = "+value);
                
                throw new Exception ("example");
        }

}


