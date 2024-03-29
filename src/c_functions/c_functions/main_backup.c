//#include <stdio.h> /* para funções de entrada saida, como printf e fopen*/
//#include <stdlib.h> /* para funções de alocação de memoria dinamica, como malloc*/
//#include <png.h> /* para a a biblioteca libpng, usada para lidar com arquivos png */
//#include <math.h>
//
////typedef struct {
//	int width;
//	int height;
//	png_byte bit_depth;
//	png_byte color_type;
//	png_bytep* row_pointers;
//	png_structp png_ptr;
//	png_infop info_ptr;
//	FILE* fp;
//}ImageInfo ;
////
////void read_png_image(const char* filename, ImageInfo* out_image) { /*
//	void -> é um tipo de retorno do C, indica que a função não vai retornar nenhum valor, a função não produz
//	nenhum resultado específico.
//
//	const -> indica que o conteudo apontado pelo ponteiro não pode ser modificado, a string não pode ser alterada
//	dentro da função
//
//	char asterisco -> estamos lidando com um ponteiro para um charactere
//
//	filename -> armazena o endereço da memoria onde a string (caminho do arquivo) está localizada
//
//	resumo -> const char asterisco filename é a descrição do tipo de dado que a variável pode apontar
//
//	diferença entre char asterisco e char[] -> char asterisco é o ponteiro para o caractere, pode apontar para uma sequencia
//	de caracteres, ja o char[] é um array de caracteres, armazena a propria sequencia de caracteres.
//	o char asterisco filename indica q estou criando um ponteiro chamado filename que aponta para uma sequencia de
//	caracteres.
//
//	char asterisco é bom para qdo a string existe.. mas para criar e manipular strings, char[] é melhor.
//
//	ou seja -> a função read_png_image recebe como argumento um ponteiro chamado filename que aponta para essa string
//	que é o caminho da imagem.
//
//	*/
//
//	FILE* fp = fopen(filename, "rb"); /*
//	FILE asterisco -> é um ponteiro para uma estrutura chamada FILE, é usado para interagir com arquivos
//	FILE -> é uma estrutura usada para representar um fluxo de arquivo, ou arquivo aberto para leitura ou gravação.
//	fp -> é o nome da variavel que armazena o ponteiro para a estrutura FILE
//
//	FILE fp ->  fp é uma variavel do tipo FILE, representa diretamente a estrutura FILE
//	não consigo realizar operações de leitura ou gravação diretamente com FILE fp, preciso de um ponteiro.
//
//	fopen(filename, 'rb') -> abre um arquivo especificado pelo filename, no modo de leitura binaria 'rb'
//	rb -> leitura binaria, r é modo de leitura (arquivo aberto como leitura) e b é modo binario
//	(le o arquivo como dados binarios)
//
//	*/
//
//	if (!fp) { /* !fp significa que se o ponteiro fp é nulo ocorre algo*/
//		perror("Erro ao abrir o arquivo"); /* esse algo é printar essa mensagem de erro */
//		return; /* nada após isso vai ocorrer, é uma proteção para que o if só mostre o erro */
//	}
//
//
//	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); /*
//	png_structp pnn_ptr -> estamos declarando uma variavel chamada png_ptr do tipo png_structp,
//	essa variavel vai ser usada para manipular arquivos png, o png_ptr também é um ponteiro.
//
//	png_create_read_struct -> estamos chamando essa funcao para criar uma estrutura de dados que nos permite ler
//	arquivos png
//
//	o argumento PNG_LIBPNG_VER_STRING -> é a versão da biblioteca libpng
//	os outros argumentos são ponteiros nulos, a função vai preencher os ponteiros com os dados necessários para a
//	manipulação de do arquivo.
//
//	*/
//
//	if (!png_ptr) { /* se o ponteiro png_ptr está vazio... significa que não foi criado a estrutura png_struct */
//		fclose(fp); /* fechamos o arquivo apontado pelo ponteiro fp */
//		fprintf(stderr, "Erro ao criar a estrutura png_struct\n"); /* imprimimos essa mensagem */
//		return;
//	}
//
//
//	png_infop info_ptr = png_create_info_struct(png_ptr); /* cria uma estrutura de infos adicionais
//	para uma imagem
//
//	estamos novamente declarando uma variavel ponteiro info_ptr que aponta para uma estrutura png_infop
//	info_ptr aponta uma estrutura png_info_struct alocada.
//
//	png_create_info_struct -> cria uma instancia da estrutura png_info_struct e a associa ao ponteiro png_ptr
//
//	png_ptr é usado para gerenciar o processo geral de criacao da imagem PNG, e o info_ptr está associado a ele para
//	armazenar infos adicionais
//
//	*/
//	if (!info_ptr) { /* se o ponteiro associado as infos adicionais estiver vazio */
//		png_destroy_read_struct(&png_ptr, NULL, NULL); /* destruimos o png_struct alocado no local onde o ponteiro png_ptr aponta
//		perceba q n qro o ponteiro png_ptr, mas sim a estrutura ligada a ele, por isso o &.
//		*/
//		fclose(fp); /* fechamos o arquivo png */
//		fprintf(stderr, "Erro ao criar a estrutura info_struct\n"); /* printamos o erro */
//		return; /* fechando o if */
//	}
//
//	if (setjmp(png_jmpbuf(png_ptr))) {
//		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
//		fclose(fp);
//		if (out_image->row_pointers != NULL) {
//			for (int y = 0; y < out_image->height; y++) {
//				free(out_image->row_pointers[y]);
//			}
//			free(out_image->row_pointers);
//		}
//		return;
//	}
//
//
//	png_init_io(png_ptr, fp); /* iniciamos a estrutura png_ptr com o arquivo alocado no ponteiro fp */
//	png_read_info(png_ptr, info_ptr); /* le as infos basicas do png, como (largura, altura, tipo de cor,
//	profundidade de bits*/
//
//	/*
//	png_struct gerencia o fluxo de gravação (entrada, saida)
//	png_info_struct permite configurar infos especificas da imagem, posso definir largura, altura, tipo de cor, etc
//
//
//	no fundo... as duas me permitem trabalhar, ler, escrever, e configurar infos especificas da imagem.
//
//	*/
//
//	int width = png_get_image_width(png_ptr, info_ptr); /* tenho a largura da imagem */
//	int height = png_get_image_height(png_ptr, info_ptr); /* tenho a altura da imagem */
//
//
//	png_byte color_type = png_get_color_type(png_ptr, info_ptr); /* qual o tipo de cor? RGB, RGBA */
//	png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr); /* profundidade de bits da imagem (qtos bits por pixel */
//
//
//	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height); 
//	if (!row_pointers) {
//		fprintf(stderr, "Erro ao alocar memória para row_pointers\n");
//		return;
//	}
//	/* aloco memoria para armazenar
//	os dados da imagem cada row_pointers[y] representa uma linha de pixels
//
//
//	png_bytep asterisco row_pointers declara um ponteiro para um array de ponteiros
//	o tipo png_bytep asterisco -> representa um ponteiro para um ponteiro do tipo png_byte, logo
//	cada linha da imagem é representada como um array de bytes
//	row_pointers sera um array de ponteiros, onde cada ponteiro aponta para uma linha de dados da imagem.
//
//	(png_bytep asterisco )malloc(sizeof(png_bytep) asterisco height) -> alocamos dinamicamente memoria no heap
//	sizeof(png_bytep) -> calcula o tamanho (em bytes) de um ponteiro png_byte (o tamanho de um ponteiro para uma linha)
//
//	a função malloc aloca essa memoria e retorna um ponteiro para o bloco de memoria alocado
//	a conversão png_bytep asterisco é usada para converter o ponteiro generico retornado por malloc
//	para o tipo especifico de ponteiro que precisamos.
//
//	o resultado disso é atribuido a row_pointers, que agora aponta para o array de ponteiros de linha
//
//
//	*/
//
//	for (int y = 0; y < height; y++) { /* para cada linha... vamos: */
//		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
//		if (!row_pointers[y]) {
//			fprintf(stderr, "Erro ao alocar memória para row_pointers[%d]\n", y);
//			return;
//		}
//		/*
//		para cada ponteiro no array row_pointers, eu vou alocar dinamicamente memoria para a linha y
//		png_get_rowbytes(png_ptr, info_ptr) -> calcula o numero de bytes necessarios para uma unica linha
//		o valor depende do formato da imagem, profundidade de bits, e qq preenchimento adicional.
//		a função malloc aloca essa memoria e retorna um ponteiro para o bloco de memoria alocado
//
//		fazemos a conversão desse ponteiro generico para o ponteiro do tipo png_byte
//
//		*/
//	}
//	/* o loop aloca memoria para cada linha de pixel com base no tamanho da linha (obtido por png_get_rowbytes
//	fazemos isso para armazenar os valores dos pixels na memoria.
//	precisamos da memoria alocada para armazenar os valors dos pixels para cada linha.
//	*/
//
//
//	png_read_image(png_ptr, row_pointers); /* e agora, finalmente estamos lendo a imagem e alocando a memoria
//	nos row_pointers */
//
//	out_image->width = width;
//	out_image->height = height;
//	out_image->bit_depth = bit_depth;
//	out_image->color_type = color_type;
//	out_image->row_pointers = row_pointers;
//	out_image->png_ptr = png_ptr;
//	out_image->info_ptr = info_ptr;
//	out_image->fp = fp;
//
//	printf("Read done with perfection.\n");
//}
//
//void dilate(png_bytep* row_pointers, int width, int height, int w, int h, png_structp png_ptr, png_infop info_ptr) {
//	png_bytep* new_row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
//	for (int y = 0; y < height; y++) {
//		new_row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
//		memcpy(new_row_pointers[y], row_pointers[y], png_get_rowbytes(png_ptr, info_ptr));
//	}
//
//	for (int y = 0; y < height; y++) {
//		for (int x = 0; x < width; x++) {
//			int max_val = 0;
//			for (int i = -w / 2; i <= w / 2; i++) {
//				for (int j = -h / 2; j <= h / 2; j++) {
//					if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
//						max_val = max(max_val, row_pointers[y + j][x + i]);
//				}
//			}
//			new_row_pointers[y][x] = max_val;
//		}
//	}
//
//	// Copiar a imagem dilatada de volta para os ponteiros de linha originais
//	for (int y = 0; y < height; y++)
//		memcpy(row_pointers[y], new_row_pointers[y], png_get_rowbytes(png_ptr, info_ptr));
//
//	// Liberar a memória alocada
//	//for (int i = 0; i < height; i++) {
//	//	free(new_row_pointers[i]);
//	//}
//	//free(new_row_pointers);
//}
//
//
//void write_png_image(ImageInfo* out_image) {
//	
//	int width = out_image->width;
//	int height = out_image ->height;
//	png_byte bit_depth = out_image->bit_depth;
//	png_byte color_type = out_image->color_type;
//	png_bytep* row_pointers = out_image->row_pointers;
//	
//	FILE* output_fp = fopen("out.png", "wb"); /* abre um arquivo chamado out.png em modo de escrita binaria */
//	if (output_fp == NULL) {
//		perror("Erro ao abrir o arquivo para escrita");
//		return 1;
//	}
//	if (!output_fp) { /* se der erro na criação desse arquivo */
//		perror("Erro ao criar o arquivo temp.png"); /* printa o erro */
//	}
//	else {
//		// Cria uma estrutura png_structp para escrever o arquivo PNG
//		png_structp png_output_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//		/* criando uma estrutura png_structp que tem o ponteiro png_outrput_ptr apontada pra mesma
//		*/
//		png_infop info_output_ptr = png_create_info_struct(png_output_ptr); /* criamos o arquivo de saida de infos
//		adicioanis */
//		png_init_io(png_output_ptr, output_fp); /* inicio a estrutura, considerando o arquivo png criado */
//
//		// Define os parâmetros da imagem de saída
//		png_set_IHDR(png_output_ptr, info_output_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
//		/* definindo os parametros, considerando o png_output_ptr,
//		as infos adicionais ein info_output_ptr
//		e as infos da minha imagem
//		como width, height, bit_depth e color_type,
//		PNG_INTERLACE_NONE: Indica que a imagem não está entrelaçada (não é uma varredura progressiva)
//		PNG_COMPRESSION_TYPE_DEFAULT: Especifica o método de compressão padrão (deflate).
//
//		PNG_FILTER_TYPE_DEFAULT: Especifica o método de filtro padrão (filtragem adaptativa).
//
//		aqui eu realmente estou criando as infos e etc.
//		*/
//
//		
//		// Escreve as informações da imagem
//		png_write_info(png_output_ptr, info_output_ptr); /* escrevendo as informações */
//
//		// Escreve os dados da imagem
//		
//		png_write_image(png_output_ptr, row_pointers); /* escrevendo os dados, que estão vindo do row_pointers */
//		
//		// Finaliza a escrita
//		png_write_end(png_output_ptr, NULL); /* escrevendo o chunk de fim para finalizar o arquivo PNG */
//
//		fclose(output_fp);
//		printf("Imagem salva como out.png\n");
//	}
//
//}
//
//void destroy_image(ImageInfo* out_image) {
//
//	int height = out_image->height;
//	png_bytep* row_pointers = out_image->row_pointers;
//	png_structp png_ptr = out_image->png_ptr;
//	png_infop info_ptr = out_image->info_ptr;
//	FILE* fp = out_image->fp;
//
//	png_byte bit_depth = out_image->bit_depth;
//	png_byte color_type = out_image->color_type;
//
//
//	// Libera a memória alocada
//	for (int y = 0; y < height; y++) {
//		free(row_pointers[y]);
//	}
//	free(row_pointers);
//
//	// Libera as estruturas
//	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
//	fclose(fp);
//
//}
//
//int main() {
//	ImageInfo image;
//
//	// Leitura Inicial 
//	const char* filename = "text.png";  
//	read_png_image(filename, &image);
//
//	// Building another ImageInfo struct to manipulate
//
//	int width = image.width;
//	int height = image.height;
//	png_byte bit_depth = image.bit_depth;
//	png_byte color_type = image.color_type;
//	png_bytep* row_pointers = image.row_pointers;
//	png_structp png_ptr = image.png_ptr;
//	png_infop info_ptr = image.info_ptr;
//	FILE* fp = image.fp;
//
//	// validating if image has only 1 channel
//	int channels = png_get_channels(png_ptr, info_ptr);
//	printf("\nImage has %d channel. \n", channels);
//
//	// creating the image array (2D)
//	//int** image_array = (int**)malloc(sizeof(int*) * height);
//	//for (int y = 0; y < height; y++) {
//	//	image_array[y] = (int*)malloc(sizeof(int) * width);
//	//	for (int x = 0; x < width; x++) {
//	//		// Atribua o valor do pixel ao array 2D
//	//		image_array[y][x] = row_pointers[y][x];
//	//	}
//	//}
//
//	printf("\nThe image has %d rows ", height);
//	printf("and %d columns. \n", width);
//
//
//	// -----------------------------------------------------------------------
//	// Threshold the image
//	// for (int y = 0; y < height; y++) {
//	// 	for (int x = 0; x < width; x++) {
//	// 		if (image_array[y][x] < 20) {
//	// 			image_array[y][x] = 255; 
//	// 		}
//	// 		else {
//	// 			image_array[y][x] = 0;
//	// 		}
//	// 		row_pointers[y][x] = image_array[y][x];
//	// 		
//	// 	}
//	// }
//	// printf("Change done. \n");
//
//	// ------------------------------------------------------------------------
//	// Rectangular adjacency relation (Dilation) 
//	int w = 2; 
//	int h = 2;
//
//	dilate(row_pointers, width, height, w, h, png_ptr, info_ptr);
//
//	//for (int y = 0; y < height; y++) {
//	//
//	//	for (int x = 0; x < width; x++) {
//	//
//	//		// finding y's
//	//		//float y_float = (float)y;
//	//		//float lim_inf_y = y_float - (float)h / 2.0;
//	//		//int lim_inf_y = (int)ceil(lim_inf_y);
//	//		//float lim_sup_y = y_float + (float)h / 2.0;
//	//		//int lim_sup_y = (int)floor(lim_sup_y);
//	//
//	//		// finding x's
//	//		//float x_float = (float)x;
//	//		//float lim_inf_x = x_float - (float)w / 2.0;
//	//		//int lim_inf_x = (int)ceil(lim_inf_x);
//	//		//float lim_sup_x = x_float + (float)w / 2.0;
//	//		//int lim_sup_x = (int)floor(lim_sup_x);
//	//
//	//
//	//		int lim_inf_y = max(0, y - h / 2);
//	//		int lim_sup_y = min(height - 1, y + h / 2);
//	//		int lim_inf_x = max(0, x - w / 2);
//	//		int lim_sup_x = min(width - 1, x + w / 2);
//	//		
//	//
//	//
//	//
//	//		int max = row_pointers[y][x];
//	//		for (int i_y = lim_inf_y; i_y <= lim_sup_y; i_y++) {
//	//			for (int i_x = lim_inf_x; i_x <= lim_sup_x; i_x++) {
//	//				if (i_y >= 0 && i_y < height && i_x >= 0 && i_x < width) {
//	//					if (row_pointers[i_y][i_x] > max) {
//	//						max = row_pointers[i_y][i_x];
//	//					}
//	//				}
//	//
//	//			}
//	//		}
//	//		//printf("%d \n", y);
//	//		//printf("%d \n", x);
//	//		row_pointers[y][x] = max;
//	//		
//	//
//	//		}
//	//	
//	//		
//	//	}
//
//	
//
//	// freeing memory from image_array
//	// for (int y = 0; y < height; y++) {
//	// 	free(image_array[y]);
//	// 	//for (int x = 0; x < width; x++) {
//	// 	//	free(image_array[y][x]);
//	// 	//}
//	// }
//	// 
//	// free(image_array);
//	
//
//	write_png_image(&image);
//	
//	destroy_image(&image);
//	return 0;
//}
//
//
//
//