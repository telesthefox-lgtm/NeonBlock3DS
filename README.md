# Neon Block 3DS

Protótipo homebrew de puzzle de blocos inspirado no gênero Block Blast, com interface neon e duas telas.

## Incluído
- Tabuleiro 10×10 na tela superior.
- Três peças selecionáveis pela touchscreen inferior.
- Colocação de peças e limpeza de linhas e colunas.
- Pontuação por peças e bônus por linhas múltiplas.
- Painéis escuros, realces brilhantes e paleta neon.
- **L + R** para reiniciar; **START** para sair.

## Importante: ainda não é um CIA
Este ZIP entrega o código-fonte e um Makefile de partida. Não inclui um `.cia` compilado. Nesta sessão não há ambiente devkitPro/libctru disponível para compilar, testar e empacotar o jogo; portanto, não seria honesto afirmar que o CIA foi gerado.

## Como compilar (quando devkitPro estiver instalado)
1. Instale devkitPro/devkitARM e os pacotes de desenvolvimento 3DS, incluindo libctru, citro3d e citro2d.
2. Abra o terminal MSYS2 fornecido pelo devkitPro.
3. Entre nesta pasta e execute `make`.
4. Corrija eventuais incompatibilidades de API/Makefile e teste o `.3dsx` no emulador ou num 3DS desbloqueado.
5. Para obter `.cia`, é necessário configurar metadados do título e usar ferramentas homebrew de empacotamento adequadas. Não instale builds não testados no console principal.

## Limitações do protótipo
- Ainda sem áudio, animações de combo, efeitos de partículas ou pré-visualização de encaixe.
- Recorde máximo apenas durante a sessão, não salvo em arquivo.
- Não testado em emulador nem em hardware real.
- A lógica e a interface precisam de compilação e testes com a versão local das bibliotecas.


## Compilação online pelo GitHub Actions (sem instalar devkitPro no Windows)

1. Crie um repositório no GitHub chamado `NeonBlock3DS`.
2. Extraia este ZIP no computador e envie **o conteúdo da pasta `NeonBlock3DS`** para a raiz do repositório. Confirme que `.github/workflows/build.yml` também foi enviado. Na interface de upload do GitHub, arquivos/pastas começando com ponto podem ser menos óbvios; se a pasta `.github` não aparecer, use GitHub Desktop ou envie o arquivo do workflow pelo editor web criando `.github/workflows/build.yml`.
3. Abra a aba **Actions** do repositório, escolha **Build NeonBlock3DS** e clique em **Run workflow**. Um push para `main`/`master` também inicia a tarefa.
4. Se a compilação passar, abra a execução e baixe o artefato `NeonBlock3DS-3dsx`.

**Importante:** este workflow tenta compilar e entrega somente `.3dsx`. Ele ainda não gera `.cia`: o projeto precisa de recursos de ícone/banner e configuração de empacotamento CIA, e a compilação inicial ainda precisa ser validada. Não renomeie um `.3dsx` para `.cia`; são formatos diferentes.
