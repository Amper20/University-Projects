#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_IO 4
#define FILE_I 3
#define STDIN_AS_I 2
#define ERR 1
#define LINE_SIZE 200
#define LOG 1
#define INITIAL_ARR_SIZE 1000
#define TYPE_D 1
#define TYPE_I 2
#define TYPE_DEL 3
  

{
  
   
   
 

{
  
   
 

init_mp (struct mp *map)
{
  
  
  
  
    {
      
      
      
      


get (struct mp *map, char *key, int type)
{
  
    {
      
	{
	  
	
    
  



set (struct mp *map, char *key, char *value, int type)
{
  
    {
      
	(struct pair *) realloc (map->lst,
				 (map->size * 2) * sizeof (struct pair));
      
	{
	  
	  
	
    
  else
    {
      
      
	{
	  
	    {
	      
	      
	    
	
      
	{
	  
	  
	  
	  
	
    



disp_map (struct mp *map)
{
  
    {
      
    


{
  
   
   
 

init_mtrx (struct matrx *input)
{
  
  
  
  
  
    {
      
      
      


add_line (struct matrx *input, char *buff)
{
  
    {
      
	(char *) realloc (input->mtrx, (input->size * 2) * sizeof (char *));
      
	{
	  
	  
	
    
  else
    {
      
      


parseD (struct mp *map, char *argv)
{
  
  
  
  
  
  
    
  
  
    
  
  else
    {
      
      
      
	{
	  
	    
	  
	
    
  
  
  
  



parseI (struct mp *map, char *argv)
{
  
  
  
  
    
  
  
  


parse_argv (int argc, char *argv[], char **in, char **out, struct mp *map) 
{
  
    {
      
    
  
    
  
  
    {
      
	{
	  
	
      else
	{
	  
	  
	
      
    
  
    {
      
	{
	  
	
      else
	{
	  
	  
	
      
    
  
    
  
    
  
    {
      
      
	{
	  
	
      
      
      
      
      
    
  else
    {
      
	{
	  
	  
	  
	  
	
    
  



get_input (int code, struct matrx *input, char *in_file_path) 
{
  
  
    {
      
    
  else
    
    {
      
    
  
  
    
  
    



print_out (int ret, struct matrx *data, char *out_file_path)
{
  
  
    
  
    
      
	
  
      else
	
  
    



check_var (char *bgn, char *en)
{
  
  
    
      
  



free_mem (char *in_file_path, char *out_file_path, struct mp *map,
	  struct matrx *input)
{
  
    
  
    
  
    {
      
	
      
      
    
  
    {
      
	{
	  
	    
	  
	    
	
      
    



solve_define (struct mp *map, struct matrx *input, int *found_main, int *i,
	      char *poz) 
{
  
  
  
  
  
    {
      
      
      
      
	{
	  
	    {
	      
		
	      
	    
	  
	  
	    
	
      
      
      
	
      
	
    
  
  
    
  
  else
    
  



solve_if (struct mp *map, struct matrx *input, int *found_main, int *i,
	  char *poz) 
{
  
  
    {
      
      
	{
	  
	  
	    {
	      
	      
		{
		  
		  
		    
		  
		
	    
	  
	  
	    {
	      
	      
		
	      
	    
	
      
    
  
  



solve_ifdef (struct mp *map, struct matrx *input, int *found_main, int *i,
	     char *poz) 
{
  
  
    {
    
  else
    {
      
      
      
      
      
	
	     && (buff[j] >= 'Z' || buff[j] <= 'A') && (buff[j] >= 'z'
						       || buff[j] <= 'a'))
	  
      
      
	// printf("%d %s\n", get(map,buff,TYPE_D), buff);
	if (get (map, buff, TYPE_D))
	{
	  
	  
	  
	  
	  
	  
	
      
      else
	
	{
	  
	  
	  
	  
	  
	  
	
    



replace_define (struct mp *map, struct matrx *input) 
{
  
  
  
    {
      
      
	
    
  
    {
      
	//check for #define
	poz = strstr (input->mtrx[i], "#define ");
      
	
      
	//check for main
	poz = strstr (input->mtrx[i], "int main()");
      
      
	
      
	//check for undef
	poz = strstr (input->mtrx[i], "#undef ");
      
	{
	  
	  
	  
	  
	  
	    
	      
	  
	
      
	//replace define
	pass = 1;
      
	{
	  
	  
	    {
	      
	      
		   && check_var (input->mtrx[i], poz) && input->prnt[i])
		{
		  
		  
		  
		  
			    strlen (poz) - strlen (map->lst[j].key));
		  
		  
		  
		
  
    {
      
      
	
    



main (int argc, char *argv[]) 
{
  
  
  
  
  
  
  
  
  
    
  
  
  
  
  
  
