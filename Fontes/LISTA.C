/***************************************************************************
*  $MCI Módulo de implementação: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.c
*  Letras identificadoras:      LIS
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
	  5		  hdpdm 28/jun/2018
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>

#define LISTA_OWN
#include "LISTA.h"
#undef LISTA_OWN

#ifdef _DEBUG
#include   "Generico.h"
#include   "Conta.h"
#include   "cespdin.h"
#include   "IdTiposEspaco.def"
#endif

/***********************************************************************
*
*  $TC Tipo de dados: LIS Elemento da lista
*
*
***********************************************************************/

   typedef struct tagElemLista {

		#ifdef _DEBUG
		
			struct LIS_tpLista * pCabeca;

			int tipo;

			int tamValor;

		#endif

         void * pValor ;
               /* Ponteiro para o valor contido no elemento */

         struct tagElemLista * pAnt ;
               /* Ponteiro para o elemento predecessor */

         struct tagElemLista * pProx ;
               /* Ponteiro para o elemento sucessor */

   } tpElemLista ;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabeça de lista
*
*
***********************************************************************/

   typedef struct LIS_tagLista {

         tpElemLista * pOrigemLista ;
               /* Ponteiro para a origem da lista */

         tpElemLista * pFimLista ;
               /* Ponteiro para o final da lista */

         tpElemLista * pElemCorr ;
               /* Ponteiro para o elemento corrente da lista */

         int numElem ;
               /* Número de elementos da lista */

         void ( * ExcluirValor ) ( void * pValor ) ;
               /* Ponteiro para a função de destruição do valor contido em um elemento */

		#ifdef _DEBUG
			
		 int tipo;

		#endif


   } LIS_tpLista ;

   /*****  Dados encapsulados no módulo  *****/

#ifdef _DEBUG

   static char EspacoLixo[256] =
	   "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
   /* Espaço de dados lixo usado ao testar */

   /*Contador de erro de estrutura*/
   int numErroEstrutura = 0;

   int totalBytes = 0;

#endif

/***** Protótipos das funções encapuladas no módulo *****/

   static void LiberarElemento( LIS_tppLista   pLista ,
                                tpElemLista  * pElem   ) ;

   static tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                       void *       pValor  ) ;

   static void LimparCabeca(LIS_tppLista pLista);

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: LIS  &Criar lista
*  ****/

   LIS_tppLista LIS_CriarLista(
             void   ( * ExcluirValor ) ( void * pDado ) )
   {

      LIS_tpLista * pLista = NULL ;
	  
      pLista = ( LIS_tpLista * ) malloc( sizeof( LIS_tpLista )) ;
      if ( pLista == NULL )
      {
         return NULL ;
	  } /* if */

	#ifdef _DEBUG
	  CED_DefinirTipoEspaco(pLista, LIS_TipoEspacoCabeca);
	  pLista->tipo = LIS_TipoEspacoCabeca;
	#endif


      LimparCabeca( pLista ) ;

      pLista->ExcluirValor = ExcluirValor ;

      return pLista ;

   } /* Fim função: LIS  &Criar lista */

/***************************************************************************
*
*  Função: LIS  &Destruir lista
*  ****/

   void LIS_DestruirLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      LIS_EsvaziarLista( pLista ) ;

      free( pLista ) ;

   } /* Fim função: LIS  &Destruir lista */

/***************************************************************************
*
*  Função: LIS  &Esvaziar lista
*  ****/

   void LIS_EsvaziarLista( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;
      tpElemLista * pProx ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif
      pElem = pLista->pOrigemLista ;
      while ( pElem != NULL )
      {
         pProx = pElem->pProx ;
         LiberarElemento( pLista , pElem ) ;
         pElem = pProx ;
      } /* while */

      LimparCabeca( pLista ) ;

   } /* Fim função: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento antes
*  ****/

   LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                           void * pValor        )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Criar elemento a inerir antes */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

      /* Encadear o elemento antes do elemento corrente */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pAnt != NULL )
            {
               pElem->pAnt  = pLista->pElemCorr->pAnt ;
               pLista->pElemCorr->pAnt->pProx = pElem ;
            } else
            {
               pLista->pOrigemLista = pElem ;
            } /* if */

            pElem->pProx = pLista->pElemCorr ;
            pLista->pElemCorr->pAnt = pElem ;
         } /* if */

         pLista->pElemCorr = pElem ;

         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento após
*  ****/

   LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                          void * pValor        )
      
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Criar elemento a inerir após */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

      /* Encadear o elemento após o elemento */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pProx != NULL )
            {
               pElem->pProx  = pLista->pElemCorr->pProx ;
               pLista->pElemCorr->pProx->pAnt = pElem ;
            } else
            {
               pLista->pFimLista = pElem ;
            } /* if */

            pElem->pAnt = pLista->pElemCorr ;
            pLista->pElemCorr->pProx = pElem ;

         } /* if */
                  
         pLista->pElemCorr = pElem ;
                  
         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Inserir elemento após */

/***************************************************************************
*
*  Função: LIS  &Excluir elemento
*  ****/

   LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista  != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      pElem = pLista->pElemCorr ;

      /* Desencadeia à esquerda */

         if ( pElem->pAnt != NULL )
         {
            pElem->pAnt->pProx   = pElem->pProx ;
            pLista->pElemCorr    = pElem->pAnt ;
         } else {
            pLista->pElemCorr    = pElem->pProx ;
            pLista->pOrigemLista = pLista->pElemCorr ;
         } /* if */

      /* Desencadeia à direita */

         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem->pAnt ;
         } else
         {
            pLista->pFimLista = pElem->pAnt ;
         } /* if */

		 LiberarElemento(pLista, pElem);

      return LIS_CondRetOK ;

   } /* Fim função: LIS  &Excluir elemento */

/***************************************************************************
*
*  Função: LIS  &Obter referência para o valor contido no elemento
*  ****/

   void * LIS_ObterValor( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
        return NULL ;
      } /* if */

      return pLista->pElemCorr->pValor ;

   } /* Fim função: LIS  &Obter referência para o valor contido no elemento */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento inicial
*  ****/

   void IrInicioLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      pLista->pElemCorr = pLista->pOrigemLista ;

   } /* Fim função: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento final
*  ****/

   void IrFinalLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      pLista->pElemCorr = pLista->pFimLista ;

   } /* Fim função: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Função: LIS  &Avançar elemento
*  ****/

   LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                              int numElem )
   {

      int i ;

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Tratar lista vazia */

         if ( pLista->pElemCorr == NULL )
         {

            return LIS_CondRetListaVazia ;

         } /* fim ativa: Tratar lista vazia */

      /* Tratar avançar para frente */

         if ( numElem > 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i > 0 ; i-- )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pProx ;
            } /* for */

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pFimLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avançar para frente */

      /* Tratar avançar para trás */

         else if ( numElem < 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i < 0 ; i++ )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pAnt ;
            } /* for */

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pOrigemLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avançar para trás */

      /* Tratar não avançar */

         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Avançar elemento */

/***************************************************************************
*
*  Função: LIS  &Procurar elemento contendo valor
*  ****/

   LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista ,
                                    void * pValor        )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista  != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      for ( pElem  = pLista->pElemCorr ;
            pElem != NULL ;
            pElem  = pElem->pProx )
      {
         if ( pElem->pValor == pValor )
         {
            pLista->pElemCorr = pElem ;
            return LIS_CondRetOK ;
         } /* if */
      } /* for */

      return LIS_CondRetNaoAchou ;

   } /* Fim função: LIS  &Procurar elemento contendo valor */


#ifdef _DEBUG
/***************************************************************************
*
*  Função: LIS  &Procurar elemento contendo valor
*  ****/

   LIS_tpCondRet LIS_VerificarLista(LIS_tppLista pCabeca, int * numFalhas) {
		
	   LIS_tpCondRet ret;
	   
	   LIS_VerificarCabeca(pCabeca);

	   CED_MarcarEspacoAtivo(pCabeca);

	   LIS_VerificarNo(pCabeca);

	   *numFalhas = numErroEstrutura;

		return LIS_CondRetOK;
	}

#endif // _DEBUG

#ifdef _DEBUG
   /***************************************************************************
   *
   *  Função: LIS  &Verificar cabeca lista
   *  ****/

   LIS_tpCondRet LIS_VerificarCabeca(LIS_tppLista pCabeca) {
		
	   LIS_tppLista pLista;

	   /*Verificar tipo espaco cabeca*/

	   if (pCabeca->tipo != CED_ObterTipoEspaco(pCabeca)) {
		   CNT_CONTAR("Tipo de espaco de dados nao é cabeca de lista");
		   numErroEstrutura++;
	   }
	   else {
		   CNT_CONTAR("Tipo de espaco de dados é cabeca de lista");
	   }

	   /*verificar origem lista*/

	   if (pCabeca->pOrigemLista != NULL) {
		   CNT_CONTAR("Origem da lista nao é NULL");
		   if (pCabeca != pCabeca->pOrigemLista->pCabeca) {
			   CNT_CONTAR("No origem lista nao aponta para cabeca");
			   numErroEstrutura++;
		   }
	   }
	   else {	   
			if (pCabeca->pElemCorr != NULL) {
				   CNT_CONTAR("Lista vazia tem no corente para nao NULL");
				   numErroEstrutura++;
			}
	   }

	   /*verificar elemento corrente*/

	   if (pCabeca->pElemCorr != NULL) {
		   CNT_CONTAR("Elemento corrente nao é NULL");
		   if (pCabeca != pCabeca->pElemCorr->pCabeca) {
			   CNT_CONTAR("No elementocorrente nao aponta para cabeca");
			   numErroEstrutura++;
			}
	   }
	   else {
		   if (pCabeca->pOrigemLista != NULL) {
			   CNT_CONTAR("Lista vazia nao tem no corrente");
			   numErroEstrutura++;
		   }
	   }

		/*verifica ultimo elemento*/
	   if (pCabeca->pFimLista != NULL) {
		   CNT_CONTAR("Elemento fim lista nao é NULL");
		   if (pCabeca != pCabeca->pFimLista->pCabeca) {
			   CNT_CONTAR("No do fimlista nao aponta para cabeca");
			   numErroEstrutura++;
			}
	   }
	   else {
		   if (pCabeca->pElemCorr != NULL) {
			   CNT_CONTAR("Lista vazia tem no corrente para nao NULL");
			   numErroEstrutura++;
			}
	   }

	   /*Verifica numero de elementos*/
	   
	   if (pCabeca->numElem < 0) {
		   CNT_CONTAR("Numero de elementos menor do que 0");
		   numErroEstrutura++;
	   
	   }
	   else if (pCabeca->numElem == 0) {
		   CNT_CONTAR("Numero de elementos igual a 0");
		   if (pCabeca->pElemCorr != NULL || pCabeca->pFimLista != NULL || pCabeca->pOrigemLista != NULL) {
			   CNT_CONTAR("Numero de elementos 0 com lista nao vazia");
			   numErroEstrutura++;
		   }
	   }
	   else if (pCabeca->numElem > 0) {
		   CNT_CONTAR("Numero de elementos maior que 0");
		   if (pCabeca->pElemCorr == NULL || pCabeca->pFimLista == NULL || pCabeca->pOrigemLista == NULL) {
			   CNT_CONTAR("Numero de elementos maior que 0 com lista vazia");
			   numErroEstrutura++;
		   }
	   }
	   CNT_CONTAR("Cabeca de lista OK");

	   return LIS_CondRetOK;
	}

#endif

#ifdef _DEBUG

   /***************************************************************************
   *
   *  Função: LIS  &DErturpar lista
   *  ****/
   LIS_tpCondRet LIS_Deturpar(LIS_tppLista pLista, LIS_tpModosDeturpacao modo)
   {
	   LIS_tpLista* lista;

	   lista = pLista;
	   switch (modo)
	   {
	   case DeturpaOrigem:
		   lista->pOrigemLista = NULL;
		   break;

	   case DeturpaFim:
		   lista->pFimLista = NULL;
		   break;

	   case DeturpaCorrente:
		   lista->pElemCorr = NULL;
		   break;

	   case DeturpaNumeroDeElementos:
		   lista->numElem++;
		   break;

	   case DeturpaNoAntes:
		   lista->pElemCorr->pAnt = NULL;
		   break;

	   case DeturpaNoProx:
		   lista->pElemCorr->pProx = NULL;
		   break;

	   case DeturpaTipoCabeca:
		   CED_DefinirTipoEspaco(lista, CED_ID_TIPO_VALOR_NULO);
		   break;

	   case DeturpaTipoElemento:
		   CED_DefinirTipoEspaco(lista->pElemCorr, CED_ID_TIPO_VALOR_NULO);
		   break;

	   case DeturpaEspacoCabeca:
		   memcpy(lista - 10, "????", 4);
		   break;

	   case DeturpaCorrenteLixo:
		   lista->pElemCorr = (LIS_tpLista*)EspacoLixo;
		   break;

	   case DeturpaFimLixo:
		   lista->pFimLista = (LIS_tpLista*)EspacoLixo;
		   break;
		
	   case DeturpaOrigemLixo:
		   lista->pOrigemLista = (LIS_tpLista*)EspacoLixo;
		   break;

	   case DeturpaCabeca:
		   lista->pElemCorr->pCabeca = NULL;
		   break;

	   case DeturpaValor:
		   memcpy(&(lista->pElemCorr->pValor), "<<<<<", 5);
		   break;

	   case DeturpaEspacoNo:
		   memcpy(((char*)lista->pElemCorr) - 50, "????????", 8);
		   break;

	   case DeturpaProxLixo:
		   lista->pElemCorr->pProx = (LIS_tpLista*)EspacoLixo;
		   break;

	   case DeturpaAntLixo:
		   lista->pElemCorr->pAnt = (LIS_tpLista*)EspacoLixo;
		   break;

	   case DeturpaNumElemNeg:
		   lista->numElem = -1;
		   break;

	   case DeturpaNumElem:
		   lista->numElem++;
		   break;
	   case DeturpaNumElemZero:
		   lista->numElem = 0;

	   case DeturpaNumBytes:
		   lista->pElemCorr->tamValor = -1;
		
	   default:
		   break;
	   }
   }
#endif
/*****  Código das funções encapsuladas no módulo  *****/

#ifdef _DEBUG
/***************************************************************************
*
*  Função: LIS  &Verificar no lista
*  ****/
   LIS_tpCondRet LIS_VerificarNo(LIS_tppLista pCabeca) {

	   /*verificar se é no estrutural*/
	   LIS_tppLista pLista;
	   tpElemLista * pElem;
	   int numElementos = 0;

	   for (pElem = pCabeca->pOrigemLista; pElem != NULL; pElem = pElem->pProx) {

		   if (pElem->tipo != CED_ObterTipoEspaco(pElem)) {
			   CNT_CONTAR("Tipo de espaco de dados nao é no de lista");
			   numErroEstrutura++;
		   }
		   else {
			   CNT_CONTAR("Tipo do espaco é no lista");
		   }

		   CED_MarcarEspacoAtivo(pElem);

		   /*verificar cabeca*/

		   if (pCabeca->pOrigemLista != NULL) {
			   CNT_CONTAR("Lista nao esta vazia");
			   if (pElem->pCabeca != pCabeca->pOrigemLista->pCabeca) {
				   CNT_CONTAR("No nao pertence a lista");
				   numErroEstrutura++;
			   }
		   }

		   /*verificar no prox*/

		   if (pElem->pProx != NULL) {
			   CNT_CONTAR("Elemento prox aponta para outro");
			   if (pElem != pElem->pProx->pAnt) {
				   CNT_CONTAR("Encadeamento prox errado");
				   numErroEstrutura++;
			   }

			   if (pCabeca != pElem->pProx->pCabeca) {
				   CNT_CONTAR("No prox nao pertence a mesma cabeca");
				   numErroEstrutura++;
			   }
		   }
		   else {
			   CNT_CONTAR("Elemento prox nao aponta para outro");
		   }

		   /*verificar no ant*/
		   if (pElem->pAnt != NULL) {
			   CNT_CONTAR("Elemento ant aponta para outro");
			   if (pElem != pElem->pAnt->pProx) {
				   CNT_CONTAR("Encadeamento no ant");
				   numErroEstrutura++;
			   }

			   if (pCabeca != pElem->pAnt->pCabeca) {
				   CNT_CONTAR("No ant nao pertence a mesma cabeca");
				   numErroEstrutura++;
			   }
		   }
		   else {
			   CNT_CONTAR("Elemento ant nao aponta para outro");
		   }

		   /*verificar tamanho bytes*/
		   if (pElem != NULL) {
			   CNT_CONTAR("Elemento nao é NULL");
			   if (pElem->tamValor != sizeof(*pElem)) {
				   CNT_CONTAR("Numero de bytes do elemento esta errado");
				   numErroEstrutura++;
			   }
		   }

		   numElementos++;
		   totalBytes += pElem->tamValor;
	   }

	   /*testa numero de elementos*/

	   if (pCabeca->numElem != numElementos) {
		   CNT_CONTAR("Numero de elementos da lista diferente do cotador da cabeca");
		   numErroEstrutura++;
	   }
	   else {
		   CNT_CONTAR("Numero de elementos é igual do contador da cobeca");
	   }

	   return LIS_CondRetOK;
   }
#endif


/***********************************************************************
*
*  $FC Função: LIS  -Liberar elemento da lista
*
*  $ED Descrição da função
*     Elimina os espaços apontados pelo valor do elemento e o
*     próprio elemento.
*
***********************************************************************/

   void LiberarElemento( LIS_tppLista   pLista ,
                         tpElemLista  * pElem   )
   {

      if ( ( pLista->ExcluirValor != NULL )
        && ( pElem->pValor != NULL        ))
      {
         pLista->ExcluirValor( pElem->pValor ) ;
      } /* if */

      free( pElem ) ;

      pLista->numElem-- ;

   } /* Fim função: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Função: LIS  -Criar o elemento
*
***********************************************************************/

   tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                void *       pValor  )
   {

      tpElemLista * pElem ;

      pElem = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
      if ( pElem == NULL )
      {
         return NULL ;
      } /* if */
	  
	 #ifdef _DEBUG
	  CED_DefinirTipoEspaco(pElem, LIS_TipoEspacoNo);
	  pElem->tipo = LIS_TipoEspacoNo;
	  pElem->pCabeca = pLista;
	  pElem->tamValor = sizeof(tpElemLista);
	 #endif

      pElem->pValor = pValor ;
      pElem->pAnt   = NULL  ;
      pElem->pProx  = NULL  ;
	  
	  pLista->numElem++;

      return pElem ;

   } /* Fim função: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Função: LIS  -Limpar a cabeça da lista
*
***********************************************************************/

   void LimparCabeca( LIS_tppLista pLista )
   {

      pLista->pOrigemLista = NULL ;
      pLista->pFimLista = NULL ;
      pLista->pElemCorr = NULL ;
      pLista->numElem   = 0 ;

   } /* Fim função: LIS  -Limpar a cabeça da lista */

/********** Fim do módulo de implementação: LIS  Lista duplamente encadeada **********/

