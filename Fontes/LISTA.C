/***************************************************************************
*  $MCI M�dulo de implementa��o: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.c
*  Letras identificadoras:      LIS
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
	  5		  hdpdm 28/jun/2018
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
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
*  $TC Tipo de dados: LIS Descritor da cabe�a de lista
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
               /* N�mero de elementos da lista */

         void ( * ExcluirValor ) ( void * pValor ) ;
               /* Ponteiro para a fun��o de destrui��o do valor contido em um elemento */

		#ifdef _DEBUG
			
		 int tipo;

		#endif


   } LIS_tpLista ;

   /*****  Dados encapsulados no m�dulo  *****/

#ifdef _DEBUG

   static char EspacoLixo[256] =
	   "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
   /* Espa�o de dados lixo usado ao testar */

   /*Contador de erro de estrutura*/
   int numErroEstrutura = 0;

   int totalBytes = 0;

#endif

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static void LiberarElemento( LIS_tppLista   pLista ,
                                tpElemLista  * pElem   ) ;

   static tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                       void *       pValor  ) ;

   static void LimparCabeca(LIS_tppLista pLista);

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: LIS  &Criar lista
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

   } /* Fim fun��o: LIS  &Criar lista */

/***************************************************************************
*
*  Fun��o: LIS  &Destruir lista
*  ****/

   void LIS_DestruirLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      LIS_EsvaziarLista( pLista ) ;

      free( pLista ) ;

   } /* Fim fun��o: LIS  &Destruir lista */

/***************************************************************************
*
*  Fun��o: LIS  &Esvaziar lista
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

   } /* Fim fun��o: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Fun��o: LIS  &Inserir elemento antes
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

   } /* Fim fun��o: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Fun��o: LIS  &Inserir elemento ap�s
*  ****/

   LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                          void * pValor        )
      
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Criar elemento a inerir ap�s */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

      /* Encadear o elemento ap�s o elemento */

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

   } /* Fim fun��o: LIS  &Inserir elemento ap�s */

/***************************************************************************
*
*  Fun��o: LIS  &Excluir elemento
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

      /* Desencadeia � esquerda */

         if ( pElem->pAnt != NULL )
         {
            pElem->pAnt->pProx   = pElem->pProx ;
            pLista->pElemCorr    = pElem->pAnt ;
         } else {
            pLista->pElemCorr    = pElem->pProx ;
            pLista->pOrigemLista = pLista->pElemCorr ;
         } /* if */

      /* Desencadeia � direita */

         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem->pAnt ;
         } else
         {
            pLista->pFimLista = pElem->pAnt ;
         } /* if */

		 LiberarElemento(pLista, pElem);

      return LIS_CondRetOK ;

   } /* Fim fun��o: LIS  &Excluir elemento */

/***************************************************************************
*
*  Fun��o: LIS  &Obter refer�ncia para o valor contido no elemento
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

   } /* Fim fun��o: LIS  &Obter refer�ncia para o valor contido no elemento */

/***************************************************************************
*
*  Fun��o: LIS  &Ir para o elemento inicial
*  ****/

   void IrInicioLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      pLista->pElemCorr = pLista->pOrigemLista ;

   } /* Fim fun��o: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Fun��o: LIS  &Ir para o elemento final
*  ****/

   void IrFinalLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      pLista->pElemCorr = pLista->pFimLista ;

   } /* Fim fun��o: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Fun��o: LIS  &Avan�ar elemento
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

      /* Tratar avan�ar para frente */

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

         } /* fim ativa: Tratar avan�ar para frente */

      /* Tratar avan�ar para tr�s */

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

         } /* fim ativa: Tratar avan�ar para tr�s */

      /* Tratar n�o avan�ar */

         return LIS_CondRetOK ;

   } /* Fim fun��o: LIS  &Avan�ar elemento */

/***************************************************************************
*
*  Fun��o: LIS  &Procurar elemento contendo valor
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

   } /* Fim fun��o: LIS  &Procurar elemento contendo valor */


#ifdef _DEBUG
/***************************************************************************
*
*  Fun��o: LIS  &Procurar elemento contendo valor
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
   *  Fun��o: LIS  &Verificar cabeca lista
   *  ****/

   LIS_tpCondRet LIS_VerificarCabeca(LIS_tppLista pCabeca) {
		
	   LIS_tppLista pLista;

	   /*Verificar tipo espaco cabeca*/

	   if (pCabeca->tipo != CED_ObterTipoEspaco(pCabeca)) {
		   CNT_CONTAR("Tipo de espaco de dados nao � cabeca de lista");
		   numErroEstrutura++;
	   }
	   else {
		   CNT_CONTAR("Tipo de espaco de dados � cabeca de lista");
	   }

	   /*verificar origem lista*/

	   if (pCabeca->pOrigemLista != NULL) {
		   CNT_CONTAR("Origem da lista nao � NULL");
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
		   CNT_CONTAR("Elemento corrente nao � NULL");
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
		   CNT_CONTAR("Elemento fim lista nao � NULL");
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
   *  Fun��o: LIS  &DErturpar lista
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
/*****  C�digo das fun��es encapsuladas no m�dulo  *****/

#ifdef _DEBUG
/***************************************************************************
*
*  Fun��o: LIS  &Verificar no lista
*  ****/
   LIS_tpCondRet LIS_VerificarNo(LIS_tppLista pCabeca) {

	   /*verificar se � no estrutural*/
	   LIS_tppLista pLista;
	   tpElemLista * pElem;
	   int numElementos = 0;

	   for (pElem = pCabeca->pOrigemLista; pElem != NULL; pElem = pElem->pProx) {

		   if (pElem->tipo != CED_ObterTipoEspaco(pElem)) {
			   CNT_CONTAR("Tipo de espaco de dados nao � no de lista");
			   numErroEstrutura++;
		   }
		   else {
			   CNT_CONTAR("Tipo do espaco � no lista");
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
			   CNT_CONTAR("Elemento nao � NULL");
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
		   CNT_CONTAR("Numero de elementos � igual do contador da cobeca");
	   }

	   return LIS_CondRetOK;
   }
#endif


/***********************************************************************
*
*  $FC Fun��o: LIS  -Liberar elemento da lista
*
*  $ED Descri��o da fun��o
*     Elimina os espa�os apontados pelo valor do elemento e o
*     pr�prio elemento.
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

   } /* Fim fun��o: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Fun��o: LIS  -Criar o elemento
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

   } /* Fim fun��o: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Fun��o: LIS  -Limpar a cabe�a da lista
*
***********************************************************************/

   void LimparCabeca( LIS_tppLista pLista )
   {

      pLista->pOrigemLista = NULL ;
      pLista->pFimLista = NULL ;
      pLista->pElemCorr = NULL ;
      pLista->numElem   = 0 ;

   } /* Fim fun��o: LIS  -Limpar a cabe�a da lista */

/********** Fim do m�dulo de implementa��o: LIS  Lista duplamente encadeada **********/

