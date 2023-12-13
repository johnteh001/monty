#define _POSIX_C_SOURCE 200809L
#include "monty.h"
#include <string.h>
#include <stdlib.h>

void fill_opcodes(instruction_t *opcodes);
void execute_opcode(char *line, stack_t **stack, unsigned int line_number, instruction_t *opcodes);
void free_opcodes(instruction_t *opcodes);

#define NUM_OF_OPCODES 2 

/**
 * main - Monty interpreter main function
 * @argc: number of command-line arguments
 * @argv: array of command-line arguments
 * Return: EXIT_SUCCESS if successful, EXIT_FAILURE on failure
 */
int main(int argc, char *argv[])
{
	FILE *file;
	char *line = NULL;
	size_t len = 0;
	unsigned int line_number = 0;
	stack_t *stack = NULL;
	instruction_t *opcodes = NULL;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		return (EXIT_FAILURE);
	}
	file = fopen(argv[1], "r");
	if (!file)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		return (EXIT_FAILURE);
	}
	opcodes = malloc(sizeof(instruction_t) * NUM_OF_OPCODES);
	if (!opcodes)
	{
		fprintf(stderr, "Error: malloc failed\n");
		fclose(file);
		return (EXIT_FAILURE);
	}
	fill_opcodes(opcodes);
	while (getline(&line, &len, file) != -1)
	{
		line_number++;
		execute_opcode(line, &stack, line_number, opcodes);
	}
	free_opcodes(opcodes);
	free_stack(stack);
	free(line);
	fclose(file);
	return (EXIT_SUCCESS);
}
/**
 * execute_opcode - Execute Monty opcode
 * @line: Line containing opcode and arguments
 * @stack: Double pointer to the stack
 * @line_number: Line number in the file
 * @opcodes: Array of supported opcodes
 */
void execute_opcode(char *line, stack_t **stack, unsigned int line_number, instruction_t *opcodes)
{
	char *opcode;
	int i;

	opcode = strtok(line, " \t\n");
	if (!opcode || opcode[0] == '#')
		return;
	for (i = 0; i < NUM_OF_OPCODES; i++)
	{
		if (strcmp(opcode, opcodes[i].opcode) == 0)
		{
			opcodes[i].f(stack, line_number);
			return;
		}
	}
	fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
	exit(EXIT_FAILURE);
}
/**
 * fill_opcodes - Fill the opcodes array with supported opcodes
 * @opcodes: Array of instruction_t structures
 */
void fill_opcodes(instruction_t *opcodes)
{
	opcodes[0].opcode = "push";
	opcodes[0].f = push;
	opcodes[NUM_OF_OPCODES - 1].opcode = NULL;
	opcodes[NUM_OF_OPCODES - 1].f = NULL;
}
/**
 * free_opcodes - Free memory allocated for opcodes array
 * @opcodes: Array of instruction_t structures
 */
void free_opcodes(instruction_t *opcodes)
{
	free(opcodes);
}
/**
 * free_stack - Free memory allocated for the stack
 * @stack: Double pointer to the stack
 */
void free_stack(stack_t **stack)
{
	stack_t *temp;

	while (*stack)
	{
		temp = *stack;
		*stack = (*stack)->next;
		free(temp);
	}
}
